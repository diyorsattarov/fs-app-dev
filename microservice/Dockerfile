# Use a specific version of Debian
FROM debian:11

# Install required packages including hiredis
RUN apt-get update && apt-get install -y \
    g++ \
    make \
    cmake \
    git \
    libboost-all-dev \
    libhiredis-dev \
    libspdlog-dev \
 && apt-get clean \
 && rm -rf /var/lib/apt/lists/*

# Set the working directory for the application
WORKDIR /app

# Copy the source code and the Makefile
COPY . /app

# Build the application
RUN cmake . && make all

# Expose the port the app runs on
EXPOSE 8080

# Command to run the executable
CMD ["./microservice", "GET", "/"]
