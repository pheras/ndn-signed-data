/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "validator.hpp"

#include <ndn-cxx/data.hpp>
#include <ndn-cxx/security/validator-config.hpp>


namespace ndn {
namespace examples {

std::string Validator::VALIDATOR_CONF_FILE = "./src/consumer-signed-data1/validator.conf";

Validator::Validator(Face& face, const std::string &confFile /* = VALIDATOR_CONF_FILE */)
  : ValidatorConfig(face)
{
  try {
    std::cout << "OPEN File= " << confFile << std::endl; 
    this->load(confFile);
  }
  catch (const std::exception &e ) {
    std::cout << "Can not load File= " << confFile << ". Error: " << e.what() 
      <<  std::endl;
    exit(1);
  }
}


void
Validator::validate(const Data& data,
                    const OnDataValidated& onValidated,
                    const OnDataValidationFailed& onValidationFailed)
{
  ValidatorConfig::validate(data,
                            [this, onValidated] (const shared_ptr<const Data>& data) {
                              this->onDataValidated(data);
                              onValidated(data);
                            },
                            [this, onValidationFailed] (const shared_ptr<const Data>& data,
                                                        const std::string& str) {
                              this->onDataValidationFailed(data, str);
                              onValidationFailed(data, str);
                            }
                            );
}

void
Validator::onDataValidated(const shared_ptr<const Data>& data)
{
  std::cout << "Validated. KeyLocator: " <<
                data->getSignature().getKeyLocator().getName() << std::endl;

}

void
Validator::onDataValidationFailed(const shared_ptr<const Data>& data, const std::string& str)
{
  std::cerr << "Validated FAILED " << data->getName() << ". due to: " << str <<
                ". KeyLocator: " << data->getSignature().getKeyLocator().getName() << 
               std::endl;
}

} // namespace examples
} // namespace ndn