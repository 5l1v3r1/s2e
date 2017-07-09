# Disclaimer
I do not develop S2E. This is simply my attempt at making it easier to setup for me, and anyone else who chooses to try this.

# Installing
There are two options to install S2E from this repo. Of course, be sure to have Docker installed.

```bash
sudo apt-get install docker
```

## Dockerfile
The Dockerfile housed in here contains the instructions to tell docker how to build your own version. If you don't trust me and simply want to build you own version, this should help.

```bash
sudo docker build --rm -t s2e .
```

Sit back. Depending on the speed of your computer this could take hours.

## Pre-build
Dockerhub has (or will have soon) a version of this docker file already build. You can simply pull it down and go.

```bash
sudo docker pull bannsec/s2e
```

# Running
Regardless of how you got it, you need to run it with a few different flags. Here's the jist:

```bash
sudo docker run -it --rm --device /dev/kvm:/dev/kvm --privileged s2e
```

A breakdown for those new to docker:

1. `run` -- We want docker to run our image
1. `-it` -- We want to have an Interactive Terminal
1. `--rm` -- Tell docker to remove the container once we exit. This is just for keeping things cleaner
1. `--device /dev/kvm:/dev/kvm` -- Let our image use kvm. This helps with speed and is required if you're building custom virtual machines in this image.
1. `--privileged` -- We need to give this container privs to allow the internal docker to run ulimit and other stuff. I think you could remove this flag if you do not intend to build virtualmachines inside this docker.
1. `s2e` -- The image tag that we gave in the above examples. If you downloaded it, use `bannsec/s2e`.

# Warning
S2E is quite large. Right now, it's looking to be about 14GB without virtual machines. You have been warned.

# Stages
You'll notice "stage1", etc here. This is due to the automated docker build process timing out. Do not worry about these.
