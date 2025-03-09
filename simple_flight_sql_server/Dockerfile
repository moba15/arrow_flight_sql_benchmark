FROM ubuntu:noble AS baseimg
RUN apt-get update && apt-get -y install software-properties-common wget
RUN add-apt-repository "deb http://apt.llvm.org/noble/ llvm-toolchain-noble-20 main"
RUN wget https://apache.jfrog.io/artifactory/arrow/$(lsb_release --id --short | tr 'A-Z' 'a-z')/apache-arrow-apt-source-latest-$(lsb_release --codename --short).deb && apt install -y -V ./apache-arrow-apt-source-latest-$(lsb_release --codename  --short).deb && rm *.deb
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - && add-apt-repository "deb http://apt.llvm.org/noble/ llvm-toolchain-noble-20 main"
RUN apt-get update && apt-get -y install python3 python3-venv python3-pip git g++ cmake ninja-build wget unzip ccache curl lsb-release wget zlib1g-dev lcov clang-20 llvm-20 libclang-20-dev llvm-20-dev libmlir-20-dev mlir-20-tools clang-tidy-20 libarrow-dev=19.*  libboost-context1.83-dev
RUN pip3 install --break-system-packages lit
RUN apt upgrade -y
RUN apt install -y -V libarrow-dev # For C++
RUN apt install -y -V libarrow-glib-dev # For GLib (C)
RUN apt install -y -V libarrow-dataset-dev # For Apache Arrow Dataset C++
RUN apt install -y -V libarrow-dataset-glib-dev # For Apache Arrow Dataset GLib (C)
RUN apt install -y -V libarrow-acero-dev # For Apache Arrow Acero
RUN apt install -y -V libarrow-flight-dev # For Apache Arrow Flight C++
RUN apt install -y -V libarrow-flight-glib-dev # For Apache Arrow Flight GLib (C)
RUN apt install -y -V libarrow-flight-sql-dev # For Apache Arrow Flight SQL C++
RUN apt install -y -V libarrow-flight-sql-glib-dev # For Apache Arrow Flight SQL GLib (C)
RUN apt install -y -V libgandiva-dev # For Gandiva C++
RUN apt install -y -V libgandiva-glib-dev # For Gandiva GLib (C)
RUN apt install -y -V libparquet-dev # For Apache Parquet C++
RUN apt install -y -V libparquet-glib-dev # For Apache Parquet GLib (C)
ENV CC=gcc CXX=g++
COPY . /flightSQLSimple
RUN cd /flightSQLSimple && rm -rf build && mkdir build && cd build && cmake .. && cmake --build .
VOLUME /database
EXPOSE 8083

ENTRYPOINT ["./flightSQLSimple/build/tools/server/simpleServer"]


