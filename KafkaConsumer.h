#ifndef KAFKA_CLIENT_CONSUMER_H__ // #include guards
#define KAFKA_CLIENT_CONSUMER_H__

#include <memory>
#include <iostream>
#include <cppkafka/cppkafka.h>

class KafkaConsumer
{
public:
    KafkaConsumer(std::string brokers, std::string groupId, 
        std::string topic, int partition=-1, int offset=-1)
    {
        // Construct the configuration
        cppkafka::Configuration config = {
            { "metadata.broker.list", brokers },
            { "group.id", groupId },
            { "enable.auto.commit", true },
            { "log.connection.close", false },
            { "fetch.message.max.bytes", 15728640}
        };

        // Build a topic configuration
        cppkafka::TopicConfiguration topic_config = {
            { "auto.offset.reset", "latest" }
        };

        // Now configure it to be the default topic config
        config.set_default_topic_configuration(topic_config);

        // Create the consumer
        consumerPtr.reset(new cppkafka::Consumer(config));

        cppkafka::TopicPartitionList partitions;
        
        // Print the assigned partitions on assignment
        consumerPtr->set_assignment_callback([](const cppkafka::TopicPartitionList& partitions) {
            std::cout << "Got assigned: " << partitions << std::endl;
        });

        // Print the revoked partitions on revocation
        consumerPtr->set_revocation_callback([](const cppkafka::TopicPartitionList& partitions) {
            std::cout << "Got revoked: " << partitions << std::endl;
        });

        // Subscribe to the topic
        consumerPtr->subscribe({ topic });

        if(partition > -1 && offset > -1)
        {
            std::cout << "Consuming messages from topic " << topic
                << " from position " << offset << std::endl;
            cppkafka::TopicPartition topicpartion(topic, partition, 0);
            topicpartion.set_offset(cppkafka::TopicPartition::OFFSET_END);
            cppkafka::TopicPartitionList partitions;
            partitions.emplace_back(topicpartion);
            consumerPtr->commit(partitions);
        }
    }

    bool GetMessage(std::string& data)
    {
        // Try to consume a message
        cppkafka::Message msg = consumerPtr->poll();
        if (msg) 
        {
            // If we managed to get a message
            if (msg.get_error()) {
                // Ignore EOF notifications from rdkafka
                if (!msg.is_eof()) {
                    std::cout << "[+] Received error notification: " << msg.get_error() << std::endl;
                }
                return false;
            }
            else 
            {
                data = msg.get_payload();
                return true;
            }
        }
        return false;
    }

private:
    std::shared_ptr<cppkafka::Consumer> consumerPtr;
};

#endif
