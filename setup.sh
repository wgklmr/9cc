docker build -t ubuntu_for_9cc .

docker run -it --mount type=bind,source="$(pwd)",target=/app ubuntu_for_9cc
