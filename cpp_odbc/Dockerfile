FROM centos:centos8.4.2105
WORKDIR .
RUN cd /etc/yum.repos.d/ 
RUN sed -i 's/mirrorlist/#mirrorlist/g' /etc/yum.repos.d/CentOS-*
RUN sed -i 's|#baseurl=http://mirror.centos.org|baseurl=http://vault.centos.org|g' /etc/yum.repos.d/CentOS-*
RUN cd /
RUN yum -y update
RUN yum -y upgrade
RUN yum -y install python3
RUN yum -y install cmake
RUN yum -y install gcc
RUN yum -y install clang
RUN yum -y install unixODBC unixODBC-devel
RUN yum -y groupinstall "Development Tools"
RUN curl https://install.duckdb.org | sh
RUN export PATH='/root/.duckdb/cli/latest':$PATH
COPY odbc/ odbc/
RUN yum -y localinstall odbc/arrow-flight-sql-odbc-driver-0.9.1.168-1.x86_64.rpm

COPY odbc/odbc.ini /etc/odbc.ini
COPY odbc/odbcinst.ini /etc/odbcinst.ini
RUN odbcinst -j
COPY testodbc testodbc/
RUN cd testodbc && mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release ..  && cmake --build . -j 6
COPY duckdb_odbc-linux-amd64.zip duckdb_odbc.zip
RUN mkdir duckdb_odbc && unzip duckdb_odbc.zip -d duckdb_odbc
VOLUME duckdb_odbc/duckdb_odbc-linux-amd64/test.db
RUN cd duckdb_odbc/duckdb_odbc-linux-amd64/ && ./unixodbc_setup.sh -s -db test.db
ENV DLS ArrowLingodDb
ENV TYPE 0
ENTRYPOINT ["/bin/sh", "-c", "./testodbc/build/testodbc \"$DLS\" \"$TYPE\""]
