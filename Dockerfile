FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    git \
    pkg-config \
    libcurl4-openssl-dev \
    libasio-dev \
    python3 \
    ca-certificates \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN mkdir -p build && cd build && cmake .. && make -j

EXPOSE 18080
ENV CC_DB_PATH=/data/foods.json
ENV CC_MEALS_DB_PATH=/data/meals.json

CMD ["./build/bin/cc_app"]
