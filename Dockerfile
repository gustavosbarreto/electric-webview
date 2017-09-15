FROM ubuntu:trusty

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && \
    apt-get install -y software-properties-common && \
    apt-add-repository -y ppa:beineri/opt-qt571-trusty && \
    apt-add-repository -y ppa:ubuntu-toolchain-r/test && \
    apt-get update && \
    apt-get install -y gcc-6 g++-6 build-essential mesa-common-dev libglu1-mesa-dev xvfb python dbus && \
    apt-get install -y ca-certificates wget && \
    apt-get install -y qt57tools qt57script qt57webengine qt57webchannel qt57websockets qt57declarative qt57x11extras

RUN rm -rf /var/lib/apt/lists/*
RUN dbus-uuidgen > /var/lib/dbus/machine-id

RUN wget -O /usr/local/bin/gosu "https://github.com/tianon/gosu/releases/download/1.9/gosu-$(dpkg --print-architecture)" && \
    chmod +x /usr/local/bin/gosu && \
    gosu nobody true

ENV QTDIR=/opt/qt57
ENV PATH=$QTDIR/bin:$PATH
ENV LD_LIBRARY_PATH=$QTDIR/lib/x86_64-linux-gnu:$QTDIR/lib:$LD_LIBRARY_PATH
ENV PKG_CONFIG_PATH=$QTDIR/lib/pkgconfig:$PKG_CONFIG_PATH

ADD . /usr/src/electric-webview

RUN mkdir /tmp/build

WORKDIR /tmp/build

RUN qmake PREFIX=/usr/local QMAKE_CXX="g++-6" /usr/src/electric-webview
RUN make -j$(nproc)
RUN make install

ENTRYPOINT ["/usr/src/electric-webview/scripts/docker-entrypoint.sh"]
