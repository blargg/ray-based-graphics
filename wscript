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
    conf.env.append_value('INCLUDES', ['./', '/usr/include/eigen3'])

    conf.env.CXXFLAGS = ["-g", "-O2", "-std=c++11"]

def build(bld):
    from waflib.Tools import waf_unit_test
    bld.add_post_fun(waf_unit_test.summary)
    bld.program(
            source='main.cpp',
            target='render',
            use='easypng common camera pathtracer raytracer obj_loader',
            lib=['png', 'gomp'])

    objects = ['easypng',
            'common',
            'ray',
            'aabb',
            'shape' ]

    for obj in objects:
        bld.objects(source=obj+'.cpp', target=obj)

    bld.objects(source='kdtree.cpp', target='kdtree', use='aabb')
    bld.objects(source='simpleObject.cpp', target='simpleObject', use='sphere solidColor')
    bld.objects(source='film.cpp', target='film', use='common easypng')
    bld.objects(source='pathtracer.cpp', target='pathtracer', use='ray film common kdtree easypng')
    bld.objects(source='raytracer.cpp', target='raytracer', use='kdtree ray common AreaLight')
    bld.objects(source='AreaLight.cpp', target='AreaLight', use='ray common')
    bld.objects(source='camera.cpp', target='camera', use='common film easypng ratracer pathtracer ray')

    bld.recurse('shapes')
    bld.recurse('file_loader')
    bld.recurse('materials')

    if bld.env.TEST:
        bld.recurse('Tests')
