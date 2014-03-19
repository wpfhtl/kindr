#!/usr/bin/env python

from __future__ import print_function

import os
import sys
import subprocess

def main(argv=sys.argv[1:]):
    working_dir = argv[0]
    tests = argv[1:]
    print("Working dir: {}".format(working_dir))
    print("Running tests: ", tests)

    test_dir = working_dir + "/test_results"
    if not os.path.exists(test_dir):
      os.makedirs(test_dir)
    
    for cmd in tests:
        test_cmd = "./" + cmd + " --gtest_output=xml:" + working_dir + "/test_results/"
        print("Running: ", test_cmd)
        rc = subprocess.call(test_cmd, cwd=working_dir, shell=True)
        if rc:
            break


if __name__ == '__main__':
    sys.exit(main())
