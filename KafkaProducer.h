#ifndef KAFKA_CLIENT_PRODUCER_H__ // #include guards
#define KAFKA_CLIENT_PRODUCER_H__

#include <memory>
#include <cppkafka/cppkafka.h>

class KafkaProducer
{
public:
    KafkaProducer(std::string brokers, std::string topic, int partition=-1)
    {
        builderPtr.reset(new cppkafka::MessageBuilder(topic));

        // Get the partition we want to write to. If no partition is provided, this will be
        // an unassigned one
        if (partition != -1) {
            builderPtr->partition(partition);
        }

        // Construct the configuration
        cppkafka::Configuration config = {
            //{"request.required.acks", -1},
            { "metadata.broker.list", brokers },
            {"queue.buffering.max.ms", 5}
        };

        // Create the producer
        kProducerPtr.reset(new cppkafka::BufferedProducer<std::string>(config));

        // Set a produce success callback
        kProducerPtr->set_produce_success_callback([](const cppkafka::Message& msg) {
            std::cout << "Successfully produced message with payload " << msg.get_payload() << std::endl;
        });

        // Set a produce failure callback
        kProducerPtr->set_produce_failure_callback([](const cppkafka::Message& msg) {
            std::cout << "Failed to produce message with payload " << msg.get_payload() << std::endl;
            // Return false so we stop trying to produce this message
            return false;
        });
    } 

    void PutMessage(std::string payload)
    {
        builderPtr->payload(payload);

        // Add the message we've built to the buffered producer
        kProducerPtr->add_message(*builderPtr.get());

        kProducerPtr->flush();
    }

private:
    std::shared_ptr<cppkafka::MessageBuilder> builderPtr;
    std::shared_ptr<cppkafka::BufferedProducer<std::string>> kProducerPtr;
};

#endif
