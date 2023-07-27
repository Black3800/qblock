FROM python:3

WORKDIR /usr/src/app

COPY ./client/requirements.txt ./
RUN pip install --no-cache-dir -r requirements.txt

COPY ./falcon falcon/
COPY ./qblock qblock/
COPY ./client .

CMD ["gunicorn", "-w 1", "-b 0.0.0.0:23614", "client:create_app()", "--timeout 600"]

EXPOSE 23614