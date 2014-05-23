top = '.'
out = 'build'

from waflib import Utils
from waflib.TaskGen import feature, before_method

def options(opt):
    opt.load('compiler_cxx waf_unit_test')
    opt.add_option('--eigen_dir', action='store', default='/usr/include/eigen3', help='Directory where eigen is installed')
    opt.add_option('--test', action='store_true', default=False, help='Set to build and run tests')

def configure(conf):
    conf.load('compiler_cxx waf_unit_test')
    conf.env.EIGEN_DIR = conf.options.eigen_dir
    conf.env.TEST = conf.options.test
    conf.env.append_value('INCLUDES', ['/usr/include/eigen3'])

    conf.env.CXXFLAGS = ["-g", "-O2", "-std=c++11"]

def build(bld):
    from waflib.Tools import waf_unit_test
    bld.add_post_fun(waf_unit_test.summary)
    bld.program(
            source='main.cpp',
            target='render',
            use='easypng common',
            lib=['png'])

    objects = ['easypng',
            'common',
            'ray',
            'aabb',
            'shape',
            'kdtree']

    for obj in objects:
        bld.objects(source=obj+'.cpp', target=obj)

    bld.recurse('shapes')

    if bld.env.TEST:
        bld.recurse('Tests')
