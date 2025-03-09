#pragma once

#include <array>
#include <arrow/flight/sql/server.h>
#include <cstdlib>
#include <ctime>
#include <random>

class SimpleFlightSQLServer : public arrow::flight::sql::FlightSqlServerBase {
public:
    arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>> GetFlightInfoPreparedStatement(const arrow::flight::ServerCallContext& context, const arrow::flight::sql::PreparedStatementQuery& command, const arrow::flight::FlightDescriptor& descriptor) override;
    arrow::Result<arrow::flight::sql::ActionCreatePreparedStatementResult> CreatePreparedStatement(const arrow::flight::ServerCallContext& context, const arrow::flight::sql::ActionCreatePreparedStatementRequest& request) override;
    arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>> DoGetPreparedStatement(const arrow::flight::ServerCallContext& context, const arrow::flight::sql::PreparedStatementQuery& command) override;
    arrow::Status ClosePreparedStatement(const arrow::flight::ServerCallContext& context, const arrow::flight::sql::ActionClosePreparedStatementRequest& request) override;

private:
    arrow::Result<std::shared_ptr<arrow::RecordBatch>> getTable(std::string handle);
    template<size_t N>
    constexpr  void createArray( std::array<int8_t, N>& a)
    {
        for (size_t i = 0; i < N; i++)
        {
            a[i] = i;
        }

    }

    template<size_t N>
    constexpr  void createArray( std::array<int64_t, N>& a)
    {
        for (size_t i = 0; i < N; i++)
        {
            a[i] = std::rand();
        }

    }

    template<size_t N>

      void createArray( std::array<std::string, N>& a, unsigned int seed)
    {
        for (size_t i = 0; i < N; i++)
        {
            a[i] = generateRandomString(seed);
        }

    }

    static  std::string generateRandomString(unsigned int seed, size_t min_length = 5, size_t max_length = 20) {
        // Random device and engine
        std::mt19937 rng(seed);

        // Random length within given bounds
        std::uniform_int_distribution<size_t> length_dist(min_length, max_length);
        size_t length = length_dist(rng);

        // Character set
        const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        std::uniform_int_distribution<size_t> char_dist(0, characters.size() - 1);

        // Generate the string
        std::string random_string;
        random_string.reserve(length);
        for (size_t i = 0; i < length; ++i) {
            random_string += characters[char_dist(rng)];
        }

        return random_string;
    }

};


