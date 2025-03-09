FROM ubuntu:noble AS baseimg
RUN apt-get update && apt-get -y install software-properties-common wget
RUN apt -y install openjdk-21-jdk
COPY target/java_adbc-1.0-SNAPSHOT.jar java_adbc_benchmark.jar
VOLUME /database
ENV JDBC_URL "url"
EXPOSE 8083
ENTRYPOINT ["/bin/sh", "-c", "java --add-opens=java.base/java.nio=ALL-UNNAMED -cp java_adbc_benchmark.jar org.example.Main"]


