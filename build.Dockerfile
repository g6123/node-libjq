FROM node:18

ARG CMAKE_VERSION=3.26.4

WORKDIR /usr/local/src

ADD https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}.tar.gz ./
RUN tar xvf cmake-${CMAKE_VERSION}.tar.gz

WORKDIR /usr/local/src/cmake-${CMAKE_VERSION}

RUN ./bootstrap \
    && make \
    && make install

WORKDIR /app

RUN corepack enable && \
    corepack prepare pnpm@latest-8 --activate

COPY pnpm-lock.yaml ./
RUN pnpm fetch

COPY package.json tsconfig.json CMakeLists.txt ./
COPY src src
RUN pnpm install --ignore-scripts

CMD pnpm run prebuild && pnpm run upload
