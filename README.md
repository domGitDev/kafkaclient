# Kafka Producer and Consumer Hearders

Kafka headers I wrote to easily integrate in multiply applications

# REQUIREMENTS

- Install librdkafka to /usr/local/librdkafka (update compile.sh if you use different path)
    - 

- clone cppkafka
    - git clone https://github.com/mfontanini/cppkafka.git
    - copy to /usr/local/cppkafka (update compile.sh if you use different path)

# COMPILE AND RUN TEST

- cd kafkaclient
- ./compile.sh
- ./SignalReceiver -b 127.0.0.1:9092 -t mytopic -g 0

# ARGUMENTS
    -b: brokers IP and port number (example above is localhost)
    -t: topic name
    -g: a group id (using consumer with same broker and topic but with different group id makes it replicate the data)

    