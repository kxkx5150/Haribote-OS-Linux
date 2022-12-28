FROM alpine:3.12

ENV IMAGE_NAME=demos

RUN apk update && \
    apk --no-cache add \
        build-base \
        nasm \
        mtools

WORKDIR /demos

CMD ["/bin/ash"]