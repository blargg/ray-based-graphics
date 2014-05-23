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
    sourcedir = str(conf.path.abspath())
    sourcedir += "/src"
    conf.env.append_value('INCLUDES', [sourcedir, '/usr/include/eigen3'])
    conf.find_program('cp', var='COPY')

    conf.env.CXXFLAGS = ["-g", "-O2", "-std=c++11"]
    conf.env.DEFINES = []

def build(bld):
    bld.recurse('src')
