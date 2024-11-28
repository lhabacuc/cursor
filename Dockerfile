FROM ubuntu:latest

RUN apt-get update && \
    apt-get install -y \
    curl \
    sudo \
    software-properties-common \
    git \
    build-essential \
    xfce4 \
    xfce4-goodies \
    xorg \
    dbus-x11 \
    x11-xserver-utils \
    supervisor \
    wget \
    xfce4-terminal

RUN wget https://downloads.sourceforge.net/project/turbovnc/3.0.1/turbovnc_3.0.1_amd64.deb && \
    dpkg -i turbovnc_3.0.1_amd64.deb && \
    apt-get install -f

RUN git clone https://github.com/novnc/noVNC.git /opt/novnc && \
    git clone https://github.com/novnc/websockify /opt/novnc/utils/websockify

RUN mkdir -p /etc/supervisor/conf.d

COPY supervisord.conf /etc/supervisor/conf.d/supervisord.conf

WORKDIR /app

EXPOSE 8080

CMD ["/usr/bin/supervisord", "-c", "/etc/supervisor/supervisord.conf"]

