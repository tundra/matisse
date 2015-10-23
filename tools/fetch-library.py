#!/usr/bin/python

import argparse
import logging
import os.path
import shutil
import time
import urllib
import zipfile


logging.basicConfig(level=logging.INFO)


# What's the age within which we'll never garbage collect a library?
_AGE_LIMIT_HOURS = 3


# Converts a human-readable size to a byte count.
def human_to_bytes(str):
  letter = str[-1].lower()
  number = float(str[:-1])
  scale = 1024 ** ['b', 'k', 'm', 'g', 't'].index(letter)
  return int(number * scale)

# Ensures that the given folder exists.
def ensure_folder(path):
  if not os.path.exists(path):
    logging.info("Creating %s", path)
    os.makedirs(path)

# Returns the size of the given file or folder, recursively if this is a folder.
def recursive_size(path):
  if os.path.isdir(path):
    result = 0
    for member in os.listdir(path):
      result += recursive_size(os.path.join(path, member))
    return result
  else:
    return os.stat(path).st_size


class FetchLibrary(object):
  
  def __init__(self, args):
    self.options = self.build_option_parser().parse_args(args[1:])
    self.container_capacity = human_to_bytes(self.options.container_capacity)

  def build_option_parser(self):
    parser = argparse.ArgumentParser()
    parser.add_argument("--base-url", default=None, help="The base to fetch from",
      required=True)
    parser.add_argument("--container", default=None, help="Where to put the library",
      required=True)
    parser.add_argument("--library", default=None, help="The library to fetch",
      required=True)
    parser.add_argument("--container-capacity", default="4g",
      help="How large we'll let the container grow")
    parser.add_argument("--marker", default=None,
      help="Optional marker file to touch after fetching")
    return parser

  # Tries to ensure that the container is within the specified capacity. Ignores
  # libraries that have been added recently to avoid deleting libraries fetched
  # by other processes and, particularly, to not delete a library we just
  # fetched ourselves.
  def garbage_collect_container(self, container):
    if not os.path.exists(container):
      return

    # List all the libraries, skipping any that are not candidate for
    # collection.
    packages = []
    for package in os.listdir(container):
      fullpath = os.path.join(container, package)
      current_time = time.time()
      package_time = os.stat(fullpath).st_mtime
      age_hours = (current_time - package_time) / 3600
      if age_hours <= _AGE_LIMIT_HOURS:
        continue
      size = recursive_size(fullpath)
      packages.append((age_hours, fullpath, size))

    # Sort the libraries by modification time, oldest last.
    packages.sort()

    # Delete older libraries until we're below the capacity.
    while sum([size for (age, path, size) in packages]) > self.container_capacity:
      (age, path, size) = packages.pop()
      logging.info("Garbage collecting %s which is %.1fh old and %sm large",
        path, age, size / (1024 ** 2))
      shutil.rmtree(path)

  # Downloads the library over HTTP if it doesn't already exist.
  def ensure_archive_downloaded(self, full_library_name, physical_path):
    if os.path.exists(physical_path):
      logging.info("Archive already exists at %s", physical_path)
      return

    # Download the zipped archive.
    zip_basename = "%s.zip" % full_library_name
    url = "%s/%s" % (self.options.base_url, zip_basename)
    zip_path = os.path.join(self.options.container, zip_basename)
    logging.info("Don't already have %s; downloading %s", physical_path, url)
    (temp_zip, headers) = urllib.urlretrieve(url)
    if headers.get("Content-Type", None) == "application/xml":
      raise Exception("Server returned XML when reading %s, probably an error." % url)

    # Unzip the archive
    ensure_folder(physical_path)
    logging.info("Unpacking %s to %s", temp_zip, physical_path)
    zipfile.ZipFile(temp_zip, "r").extractall(physical_path)

    # Clean up
    logging.info("Deleting %s", temp_zip)      
    os.unlink(temp_zip)
    self.garbage_collect_container(self.options.container)

  def touch_file(self, marker):
    if not (marker is None):
      with open(marker, "a"):
        os.utime(marker, None)

  # Main entry-point
  def main(self):
    library_name = self.options.library
    full_dest_path = os.path.join(self.options.container, library_name)
    self.ensure_archive_downloaded(library_name, full_dest_path)
    self.touch_file(self.options.marker)

if __name__ == '__main__':
  import sys
  FetchLibrary(sys.argv).main()
