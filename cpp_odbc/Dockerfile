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
RUN yum -y install wget
RUN yum install -y sqlite sqlite-devel
#RUN yum -y install valgrind
RUN curl https://install.duckdb.org | sh
RUN export PATH='/root/.duckdb/cli/latest':$PATH
COPY odbc/ odbc/
RUN yum -y localinstall odbc/arrow-flight-sql-odbc-driver-0.9.1.168-1.x86_64.rpm
RUN git clone https://github.com/SAP/odbc-cpp-wrapper && cd odbc-cpp-wrapper && git checkout 6a1bb63 && mkdir build && cd build && cmake .. && make install
COPY odbc/odbc.ini /etc/odbc.ini
COPY odbc/odbcinst.ini /etc/odbcinst.ini
RUN odbcinst -j
COPY testodbc testodbc/
RUN cd testodbc && mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release ..  && cmake --build . 
COPY duckdb_odbc-linux-amd64.zip duckdb_odbc.zip
RUN mkdir duckdb_odbc && unzip duckdb_odbc.zip -d duckdb_odbc
VOLUME /database
VOLUME /valgrind_export
VOLUME /perf_export
RUN wget http://www.ch-werner.de/sqliteodbc/sqliteodbc-0.9999.tar.gz && tar -xvzf sqliteodbc-0.9999.tar.gz
RUN cd sqliteodbc-0.9999 && ./configure && make && make install
RUN cd duckdb_odbc/duckdb_odbc-linux-amd64/ && ./unixodbc_setup.sh -s
ENV DLS ArrowLingodDb
ENV TYPE 0


#ENTRYPOINT ["/bin/sh", "-c", "valgrind --log-file=\"/valgrind_export/odbc_valgrind-%p.dat\" --tool=memcheck --leak-check=yes ./testodbc/build/testodbc \"$DLS\" \"$TYPE\""]
ENTRYPOINT ["/bin/sh", "-c", "./testodbc/build/testodbc \"$DLS\" \"$TYPE\""]
#ENTRYPOINT ["/bin/sh", "-c", "perf stat -e task-clock,cycles,instructions,cache-references,cache-misses -o /perf_export/perf.text ./testodbc/build/testodbc \"$DLS\" \"$TYPE\""]