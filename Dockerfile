# Stage 1: Build
FROM debian:bookworm AS build

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    build-essential \
    g++ \
    make && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN make re


FROM debian:bookworm

WORKDIR /app

COPY --from=build /app/ /app/

RUN chmod +x /app/webserv

CMD ["/app/webserv", "/app/configs/config.conf"]
