from waflib.Build import BuildContext

top = '.'
out = 'build'


def options(opt):
    opt.load('compiler_cxx waf_unit_test')
    opt.add_option('--eigen_dir',
                   action='store',
                   default='/usr/include/eigen3',
                   help='Directory where eigen is installed')
    opt.add_option('--test',
                   action='store_true',
                   default=False,
                   help='Set to build and run tests')


def copy_env(conf, name):
    """DON'T USE THIS. copies the environment, but changes the set environment
    in the process"""
    conf.setenv(name)
    new_env = conf.env.derive()
    new_env.detach()
    return new_env


def configure(conf):
    conf.setenv('common')
    conf.load('compiler_cxx waf_unit_test')
    conf.env.EIGEN_DIR = conf.options.eigen_dir
    conf.env.TEST = conf.options.test
    sourcedir = str(conf.path.abspath()) + "/src"
    conf.env.append_value('INCLUDES', [sourcedir, '/usr/include/eigen3'])
    conf.find_program('cp', var='COPY')

    conf.env.CXXFLAGS = ["-g", "-std=c++11"]
    conf.env.DEFINES = []

    # release settings
    conf.setenv('release', copy_env(conf, 'common'))
    conf.env.CXXFLAGS += ['-O2']
    conf.env.DEFINES += ['NDEBUG']

    # debug settings
    conf.setenv('debug', copy_env(conf, 'common'))


def build(bld):
    bld.recurse('src')


class release(BuildContext):
    cmd = 'build'
    variant = 'release'


class debug(BuildContext):
    """Builds the debug version (no optimization or error checking)"""
    cmd = 'debug'
    variant = 'debug'
