FROM ubuntu:noble AS baseimg
RUN apt-get update && apt-get -y install software-properties-common wget
RUN apt -y install openjdk-21-jdk
COPY target/java_jdbc_benchmark-1.0-SNAPSHOT.jar java_jdbc_benchmark.jar
COPY OracleLog.properties OracleLog.properties
VOLUME /database
ENV JDBC_URL "url"
EXPOSE 8083
ENTRYPOINT ["/bin/sh", "-c", "java --add-opens=java.base/java.nio=ALL-UNNAMED -cp java_jdbc_benchmark.jar de.bachmaiers.Main"]


