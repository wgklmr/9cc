if [[ "$(docker images -q ubuntu_for_9cc 2> /dev/null)" == "" ]]; then
  docker build -t ubuntu_for_9cc .
fi


docker run -it --mount type=bind,source="$(pwd)",target=/app ubuntu_for_9cc
