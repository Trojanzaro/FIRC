FROM gcc:latest

COPY server.c /opt

WORKDIR /opt

RUN [ "gcc", "server.c", "-o", "server.so", "-pthread" ]

CMD [ "./server.so" ]

EXPOSE 1234
