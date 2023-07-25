FROM mongo

WORKDIR /usr/src/app

COPY setup.bash .

COPY --from=ghcr.io/ufoscout/docker-compose-wait:latest /wait /wait

CMD bash setup.bash