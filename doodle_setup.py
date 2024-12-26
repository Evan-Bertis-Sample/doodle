# doodlesetup.py

import setuptools
import os

install_requires = []
if os.path.exists("requirements.txt"):
    with open("requirements.txt", "r") as f:
        install_requires = f.read().splitlines()

setuptools.setup(
    name="doodle",
    version="0.0.1",
    author="Evan Bertis-Sample",
    install_requires=install_requires,
    entry_points={
        'console_scripts': [
            'doodle = doodle:main'
        ]
    },
    packages=setuptools.find_packages(),
    include_package_data=True,
)
