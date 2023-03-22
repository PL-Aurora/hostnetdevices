from setuptools import Extension
from distutils.core import setup

hostnetdeviceModule = Extension('HostNetDevice',
                                define_macros=[('MAJOR_VERSION', '1'),
                                               ('MINOR_VERSION', '0')],
                                include_dirs=['inc'],
                                language="C",
                                sources=['demo.c', 'src/netdevice.c', 'src/netgateway.c'])

setup(name='hostnetdevice',
      version='1.0',
      description='Small library to get info about network devices',
      author='Adrian',
      author_email='adrian_lukaszyk@wp.pl',
      ext_modules=[hostnetdeviceModule])
