FROM python:3

WORKDIR /usr/src/app

COPY ./miner/requirements.txt ./
RUN pip install --no-cache-dir -r requirements.txt

COPY ./falcon falcon/
COPY ./qblock qblock/
COPY ./miner .

# COPY --from=ghcr.io/ufoscout/docker-compose-wait:latest /wait /wait

# CMD /wait && python main.py

CMD python miner.py