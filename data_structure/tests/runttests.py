#! /usr/bin/python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
# Last modified: 2017-10-22 02:34:51

import os
import subprocess
import glob


def find_all_tests(excludes=[]):
    tests = list(map(lambda p: p.strip('/').split('/')[-1], sorted(glob.glob('tests/test_*'))))

    for exclude in excludes:
        if exclude in tests:
            tests.remove(exclude)

    for test in tests:
        if '.c' in test:
            tests.remove(test)
    return tests


def run_tests():
    tests = find_all_tests()
    for test in tests:
        subprocess.Popen('./tests/'+test)


if __name__ == "__main__":
    print("Running unit tests:")
    run_tests()
