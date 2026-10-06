// imebra_tests.cpp : Defines the entry point for the console application.
//
#include <gtest/gtest.h>
#include <nds3/nds.h>
#include <nds3/ndsTestFactory.h>

int main(int argc, char **argv)
{
    nds::Factory testControlSystem(std::shared_ptr<nds::FactoryBaseImpl>(new nds::tests::TestControlSystemFactoryImpl()));

    nds::FactoryBaseImpl::loadDriver("../../../lib/libnds3-sdn-publisher.so");
    nds::FactoryBaseImpl::loadDriver("../../../lib/libnds3-DeviceTesting.so");

    nds::Factory::registerControlSystem(testControlSystem);

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
