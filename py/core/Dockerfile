FROM python:3

WORKDIR /usr/src/app

COPY requirements.txt ./
RUN pip install --no-cache-dir -r requirements.txt

COPY . .

# COPY --from=ghcr.io/ufoscout/docker-compose-wait:latest /wait /wait

# CMD /wait && python main.py

CMD python main.py