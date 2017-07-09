FROM bannsec/s2e_stage1

USER s2e
WORKDIR /home/s2e

# This can take a while
RUN bash -c ". .virtualenvs/s2e/bin/activate && (echo 'y') | s2e init /home/s2e"

CMD /bin/bash 
