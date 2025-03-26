FROM ubuntu:noble AS baseimg
RUN apt-get update && apt-get -y install software-properties-common wget
ENV CONDA_DIR /opt/conda
RUN wget --quiet https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh -O ~/miniconda.sh &&  /bin/bash ~/miniconda.sh -b -p /opt/conda

# Put conda in path so we can use conda activate
ENV PATH=$CONDA_DIR/bin:$PATH
RUN conda config --add channels conda-forge
RUN conda update -y -n base --all
RUN conda install -y -n base mamba
RUN mamba install -y cmake compilers libadbc-driver-manager libadbc-driver-sqlite

VOLUME /database
ENV JDBC_URL "url"
ENV PATH=$CONDA_DIR/bin:$PATH
EXPOSE 8083



