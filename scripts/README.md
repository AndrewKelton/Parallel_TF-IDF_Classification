# _scripts_

_scripts_ contains `bash` and `python` code that are NOT used for the project itself and are JUST for setting up and cleaning the workspace, and creating graphs out of data.   

## About _scripts_
There is NO code in here that is essential to the Parallel TF-IDF Vectorization or Text Classification. You can run this program without using any of these scripts. However, `setup.sh` is very helpful to avoid dealing with missing directories and quickly installing dependencies. Although you can git clone the dependencies yourself, it is already automated. Directories can also be created in the `Makefile` (it is commented out), but the `Makefile` will constantly create and overwrite directories containing previous data which may not be the specified action.

## WARNING
It is essential to not change `cleanup.sh` and `setup.sh`. They set up the workspace/directory to allow continuous testing and minimal deletion of directories. `cleanup.sh` is meant to be run after all testing is completed and you would like to completely 'clean' out your project directory and dependencies.