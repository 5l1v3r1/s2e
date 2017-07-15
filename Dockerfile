FROM ubuntu:16.04

# Best if run with KVM enabled inside the Docker image
# https://github.com/dhiltgen/docker-machine-kvm
# sudo docker run -it --rm --device /dev/kvm:/dev/kvm --privileged s2e

# Test just building the plugins
# make -f ../../source/s2e/libs2eplugins/src/Makefile S2EPREFIX=/home/s2e/s2e/install -j8

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
RUN echo "cd s2e" >> ~/.bashrc
RUN echo "function s2e_rebuild { pushd .; cd /home/s2e/s2e/build/s2e; rm stamps/libs2e-*; CMAKE_PREFIX_PATH=llvm-release/cmake/modules/CMakeFiles:/home/s2e/s2e/source/s2e/libs2eplugins S2EPREFIX=/home/s2e/s2e/install make -f ../../source/s2e/Makefile install; find -name libs2eplugins -exec cp -r /home/s2e/s2e/source/s2e/libs2eplugins/* {}/.  \;; CMAKE_PREFIX_PATH=llvm-release/cmake/modules/CMakeFiles:/home/s2e/s2e/source/s2e/libs2eplugins S2EPREFIX=/home/s2e/s2e/install make -f ../../source/s2e/Makefile install; popd;}" >> ~/.bashrc
RUN echo "echo -e 'Be sure to run this docker image like this:\n\tsudo docker run -it --rm --device /dev/kvm:/dev/kvm --privileged s2e'" >> ~/.bashrc

# Make KVM accessable to S2E
RUN sudo chmod 666 /dev/kvm

CMD /bin/bash 
