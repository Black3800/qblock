FROM python:3

WORKDIR /usr/src/app

COPY requirements.txt ./
RUN pip install --no-cache-dir -r requirements.txt

COPY . .

CMD ["gunicorn", "-w 1", "-b 0.0.0.0:23714", "main:create_app()", "--timeout 600"]

EXPOSE 23714