# Use a base image with a C++ environment
FROM debian:11

# Install dependencies excluding cmake
RUN apt-get update && apt-get install -y \
    g++ \
    make \
    git \
    wget \
    libspdlog-dev

# Install the latest CMake
RUN apt-get install -y wget
RUN wget https://github.com/Kitware/CMake/releases/download/v3.25.0/cmake-3.25.0-Linux-x86_64.sh \
    && chmod +x cmake-3.25.0-Linux-x86_64.sh \
    && ./cmake-3.25.0-Linux-x86_64.sh --skip-license --prefix=/usr/local \
    && rm cmake-3.25.0-Linux-x86_64.sh

# Clone and build hiredis
RUN git clone https://github.com/redis/hiredis.git && \
    cd hiredis && \
    cmake . && \
    make install

RUN ldconfig 

# Clone and build redis++
RUN git clone https://github.com/sewenew/redis-plus-plus.git && \
    cd redis-plus-plus && \
    mkdir build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release -DREDIS_PLUS_PLUS_CXX_STANDARD=17 .. && \
    cmake . && \
    make install

# Set the working directory for the application
WORKDIR /app

# Copy the source code and the CMakeLists.txt
COPY . /app

# Build the application, linking against redis++
RUN cmake . && make all

# Expose the port the app runs on
EXPOSE 6379

# Command to run the executable with arguments
CMD ["./redis-service", "SET", "my_key", "my_value"]

CMD ["./redis-service", "GET", "my_key"]
