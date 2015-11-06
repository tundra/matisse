#!/usr/bin/python

import os
import os.path
import platform
import subprocess
import zipfile
import argparse
import shutil
import logging


logging.basicConfig(level=logging.WARN)


README="""\
This is a binary distribution of the skia graphics library; see
http://skia.org for details. This distribution is covered by the
license provided in the LICENSE file.
"""


RELEASE = True
ALWAYS = {}
LINUX_ONLY = {"system": "linux"}
WINDOWS_ONLY = {"system": "windows"}

# Names of libraries
LIBNAMES = [
  ("skia_matisse", ALWAYS),
]

def get_libfile(name):
  if platform.system() == "Windows":
    return "%s.lib" % name
  else:
    (head, tail) = os.path.split(name)
    return os.path.join(head, "lib%s.a" % tail)

def ensure_folder(name):
  if not os.path.exists(name):
    os.makedirs(name)

_SYSTEMS = {"Windows": "win", "Linux": "lnx"}

class Bundler(object):

  def __init__(self, args):
    self.options = self.build_option_parser().parse_args(args[1:])
    self.platform = {"system": platform.system().lower()}

  def build_option_parser(self):
    parser = argparse.ArgumentParser()
    parser.add_argument("--skia-base", default=None, help="The skia base directory",
      required=True)
    parser.add_argument("--outroot", default=".", help="Where to put the library")
    parser.add_argument("--mode", default="Release", help="Which mode to bundle, Release or Debug")
    parser.add_argument("--version", default=None, help="The version (M_m_b)",
      required=True)
    return parser

  def get_archive_name(self):
    raw_system = platform.system()
    system = _SYSTEMS.get(raw_system, raw_system)
    is_64bits = sys.maxsize > 2**32
    bits = "64" if is_64bits else "32"
    mode = "dbg" if self.options.mode.startswith("Debug") else "rel"
    return "libskia-%s-%s-%s-%s" % (system, bits, mode, self.options.version)

  def main(self):
    binout = os.path.join(self.options.skia_base, 'out', self.options.mode)
    libs = list(self.get_input_libraries(binout))
    for lib in libs:
      if not os.path.exists(lib):
        raise Exception("Couldn't find %s" % lib)
    merge_static_libs_py = os.path.join(self.options.skia_base, "tools", "merge_static_libs.py")
    archname = self.get_archive_name()
    outpath = os.path.join(self.options.outroot, archname)
    libout = os.path.join(outpath, "lib", get_libfile("skia"))
    if os.path.exists(outpath):
      logging.info("Deleting existing %s", outpath)
      shutil.rmtree(outpath)
    ensure_folder(os.path.dirname(libout))
    logging.info("Creating %s", libout)
    merge_command = ["python", merge_static_libs_py, libout] + libs
    print " ".join(merge_command)
    subprocess.check_call(merge_command)
    include_src = os.path.join(self.options.skia_base, "include")
    include_dest = os.path.join(outpath, "include")
    logging.info("Copying %s to %s", include_src, include_dest)
    shutil.copytree(include_src, include_dest)
    license_src = os.path.join(self.options.skia_base, "LICENSE")
    license_dest = os.path.join(outpath, "LICENSE")
    logging.info("Copying %s to %s", license_src, license_dest)
    shutil.copy(license_src, license_dest)
    readme_dest = os.path.join(outpath, "README")
    logging.info("Creating %s", readme_dest)
    with open(readme_dest, "at") as f:
      f.write(README)
    logging.info("Linux zip: cd %s && zip -r9 ../%s.zip *", outpath, archname)
    return 0

  def get_input_libraries(self, root):
    for (name, restrictions) in LIBNAMES:
      should_yield = True
      for (key, value) in restrictions.iteritems():
        current = self.platform.get(key, None)
        if current != value:
          should_yield = False
      if should_yield:
        yield os.path.join(root, get_libfile(name))


def bundle_libraries():
  abspaths = list(get_input_libraries())
  for path in abspaths:
    if not os.path.exists(path):
      print "Couldn't find %s" % path
      return
  outname = get_outfile()
  zipname = "%s.zip" % outname
  print "Merging %s into %s and %s" % (", ".join(abspaths), outname, zipname)
  if os.path.exists(outname):
    os.remove(outname)
  if subprocess.call(["python", "tools/merge_static_libs.py", outname] + abspaths) != 0:
    return
  if os.path.exists(zipname):
    os.remove(zipname)
  zipped = zipfile.ZipFile(zipname, compression=zipfile.ZIP_DEFLATED, mode="w")
  zipped.write(outname)
  zipped.close()

if __name__ == "__main__":
  import sys
  sys.exit(Bundler(sys.argv).main())

