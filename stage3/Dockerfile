FROM bannsec/s2e_stage2

USER s2e
WORKDIR /home/s2e

RUN bash -c ". .virtualenvs/s2e/bin/activate && cd s2e && s2e build"

# Give user the appropriate groups
RUN sudo usermod -a -G docker s2e
RUN sudo usermod -a -G kvm s2e
RUN sudo usermod -a -G libvirtd s2e

RUN echo "source ~/.virtualenvs/s2e/bin/activate" >> ~/.bashrc
# Sadly, can't find priviledged mode for docker build. This has to be done run-time.
RUN echo "sudo /etc/init.d/docker start" >> ~/.bashrc

CMD /bin/bash 
