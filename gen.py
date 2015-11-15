#!/usr/bin/env python2.7

from __future__ import print_function
import argparse
import sys

def gen_functions(args, out):
    for i in range(0, args.count):
        args.output.write('__attribute__((used)) int func_{0}(int x) '
                          '{{ return x + {0}; }}\n'.format(i))

def gen_classes(args, inherit):
    out = args.output
    out.write('static volatile int v;\n')
    if inherit:
        out.write('class class_0 { };\n')
        for i in range(1, args.count):
            out.write('class class_{0} : public class{1} {{ }};\n'.format(i, i-1))
    else:
        for i in range(0, args.count):
            out.write('class class_{0} {{ }};\n'.format(i))

    for i in range(0, args.count):
        out.write('void test_classes_{0}()\n'
                  '{{\n'
                  '    try {{\n'
                  '        throw class_{0}();\n'
                  '    }}\n'.format(i))
        for j in range(args.count-1, -1, -1):
            out.write('    catch (class_{0} &) {{\n'
                      '        v = {0};\n'
                      '    }}\n'.format(j))
        out.write('}\n\n')


def gen_calls(args):
    out = args.output
    out.write('static volatile int v;\n')
    out.write('__attribute__((noinline)) void call_0(void);\n')
    for i in range(1, args.count):
        out.write('__attribute__((noinline)) void call_{0}() {{\n'
                  '    v = {0};\n'
                  '    call_{1}();\n'
                  '    v = {1};\n'
                  '}}\n\n'.format(i, i-1))
        out.write('void test_call_{0}() {{\n'
                  '    try {{;\n'
                  '        call_{1}();\n'
                  '    }} catch (int) {{ }}\n'
                  '}}\n\n'.format(i, i-1))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('type', choices=['func', 'classes', 'hierarchy', 'calls'],
                        help='type of item')
    parser.add_argument('--output', '-o', type=argparse.FileType('w'),
                        default=sys.stdout,
                        help='output file (default: stdout)')
    parser.add_argument('--count', '-c', type=int,
                        help='number of items to generate', default=10)
    args = parser.parse_args()
    if args.type == 'func':
        gen_functions(args)
    elif args.type == 'classes':
        gen_classes(args, False)
    elif args.type == 'hierarchy':
        gen_classes(args, True)
    elif args.type == 'calls':
        gen_calls(args)
    else:
        assert(False)


if __name__ == '__main__':
    main()
