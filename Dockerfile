FROM ubuntu:16.04

# Best if run with KVM enabled inside the Docker image
# https://github.com/dhiltgen/docker-machine-kvm
# sudo docker run -it --rm --device /dev/kvm:/dev/kvm --privileged s2e

RUN dpkg --add-architecture i386
RUN apt-get update && apt-get -y upgrade
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y sudo virtualenvwrapper git gcc python-dev software-properties-common lsb-release vim keyboard-configuration console-setup docker

RUN useradd -m s2e

RUN echo 's2e ALL=(ALL) NOPASSWD:ALL' | sudo EDITOR='tee -a' visudo

USER s2e
WORKDIR /home/s2e

# Initial s2e setup
RUN git clone https://github.com/s2e/s2e-env.git
RUN mkdir .virtualenvs
RUN virtualenv --python=$(which python2) .virtualenvs/s2e
RUN bash -c ". .virtualenvs/s2e/bin/activate && cd s2e-env && pip install ."
RUN bash -c ". .virtualenvs/s2e/bin/activate && (echo 'y') | s2e init /home/s2e/s2e"
RUN bash -c ". .virtualenvs/s2e/bin/activate && cd s2e && s2e build"

RUN sudo apt-get install -y libvirt-bin qemu-kvm

# Give user the appropriate groups
RUN sudo usermod -a -G docker s2e
RUN sudo usermod -a -G kvm s2e
RUN sudo usermod -a -G libvirtd s2e

RUN echo "source ~/.virtualenvs/s2e/bin/activate" >> ~/.bashrc
# Sadly, can't find priviledged mode for docker build. This has to be done run-time.
RUN echo "sudo /etc/init.d/docker start" >> ~/.bashrc


CMD /bin/bash 
