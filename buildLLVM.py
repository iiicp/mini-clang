import os

current_work_dir = os.path.dirname(__file__)

workdir = os.getcwd() + "/cmake-build-debug"
os.makedirs(os.path.dirname(workdir), exist_ok=True)


