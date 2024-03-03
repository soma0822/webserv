FROM ubuntu:24.04

WORKDIR /app

RUN set -eux \
  && apt-get update \
  && apt-get install -y --no-install-recommends \
  make \
  build-essential

ENTRYPOINT ["bash"]
