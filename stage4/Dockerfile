FROM bannsec/s2e_stage3

USER s2e
WORKDIR /home/s2e

# Build LLVM
RUN bash -c ". .virtualenvs/s2e/bin/activate && cd s2e/build && make -f ../source/s2e/Makefile S2EPREFIX=/home/s2e/s2e/install stamps/llvm-release-make"

CMD /bin/bash 
