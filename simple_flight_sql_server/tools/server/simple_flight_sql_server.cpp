//
// Created by mor on 26.01.25.
//

#include "simple_flight_sql_server.h"
#include <arrow/api.h>
#include <iostream>
arrow::Status startServer();

int main()
{
    if (startServer().ok())
    {
        return 0;
    }

    return EXIT_FAILURE;
}

arrow::Status startServer()
{
    std::srand(std::time(nullptr));
    SimpleFlightSQLServer server{};
    arrow::flight::Location server_location;
    ARROW_ASSIGN_OR_RAISE(server_location, arrow::flight::Location::ForGrpcTcp("0.0.0.0", 8083));
    arrow::flight::FlightServerOptions options(server_location);
    ARROW_RETURN_NOT_OK(server.Init(options));
    ARROW_RETURN_NOT_OK(server.Serve());
}


arrow::Result<std::unique_ptr<arrow::flight::FlightInfo>> SimpleFlightSQLServer::GetFlightInfoPreparedStatement(
    const arrow::flight::ServerCallContext& context, const arrow::flight::sql::PreparedStatementQuery& command,
    const arrow::flight::FlightDescriptor& descriptor)
{
    std::vector endpoints{
        arrow::flight::FlightEndpoint{{descriptor.cmd}, {}, std::nullopt, ""}
    };
    ARROW_ASSIGN_OR_RAISE(auto result,
                          arrow::flight::FlightInfo::Make(*arrow::flight::sql::SqlSchema::GetCatalogsSchema(),
                              descriptor, endpoints, -1, -1, false));
    return std::make_unique<arrow::flight::FlightInfo>(result);
}

arrow::Result<arrow::flight::sql::ActionCreatePreparedStatementResult> SimpleFlightSQLServer::
CreatePreparedStatement(const arrow::flight::ServerCallContext& context,
                        const arrow::flight::sql::ActionCreatePreparedStatementRequest& request)
{
    return arrow::flight::sql::ActionCreatePreparedStatementResult{
        arrow::flight::sql::SqlSchema::GetCatalogsSchema(), nullptr, request.query
    };
}

arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>> SimpleFlightSQLServer::DoGetPreparedStatement(
    const arrow::flight::ServerCallContext& context, const arrow::flight::sql::PreparedStatementQuery& command)
{
    ARROW_ASSIGN_OR_RAISE(auto rBatch, getTable(command.prepared_statement_handle))
    std::vector<std::shared_ptr<arrow::RecordBatch>> batches = {rBatch};
    ARROW_ASSIGN_OR_RAISE(auto rBatchReader, arrow::RecordBatchReader::Make(
        batches, rBatch->schema()))

    // Create a FlightDataStream from the RecordBatchReader
    auto stream = std::unique_ptr<arrow::flight::FlightDataStream>(
     std::make_unique<arrow::flight::RecordBatchStream>(rBatchReader));


    return std::move(stream);
}

arrow::Status SimpleFlightSQLServer::ClosePreparedStatement(const arrow::flight::ServerCallContext& context,
    const arrow::flight::sql::ActionClosePreparedStatementRequest& request)
{
    return arrow::Status::OK();
}

arrow::Result<std::shared_ptr<arrow::RecordBatch>> SimpleFlightSQLServer::getTable(std::string handle)
{
    std::cout << handle << std::endl;
    if (!two_rows)
    {
        if (handle == "query1")
    {
        arrow::Int64Builder int64Builder;
        std::array<int64_t, 1> values{};
        createArray<1>(values);
        // AppendValues, as called, puts 5 values from days_raw into our Builder object.
        ARROW_RETURN_NOT_OK(int64Builder.AppendValues(values.begin(), 1));
        std::shared_ptr<arrow::Array> days;
        ARROW_ASSIGN_OR_RAISE(days, int64Builder.Finish());
        std::shared_ptr<arrow::Field> field_day;
        std::shared_ptr<arrow::Schema> schema;

        // Every field needs its name and data type.
        field_day = arrow::field("Day", arrow::int64());

        // The schema can be built from a vector of fields, and we do so here.
        schema = arrow::schema({field_day});

        std::shared_ptr<arrow::RecordBatch> rbatch;
        // The RecordBatch needs the schema, length for columns, which all must match,
        // and the actual data itself.
        rbatch = arrow::RecordBatch::Make(schema, days->length(), {days});
        return rbatch;
    }
    if (handle == "query2")
    {
        arrow::Int64Builder int64builder;
        std::array<int64_t, 10> days_raw{};
        createArray<10>(days_raw);
        // AppendValues, as called, puts 5 values from days_raw into our Builder object.
        ARROW_RETURN_NOT_OK(int64builder.AppendValues(days_raw.begin(), 10));
        std::shared_ptr<arrow::Array> days;
        ARROW_ASSIGN_OR_RAISE(days, int64builder.Finish());
        std::shared_ptr<arrow::Field> field_day;
        std::shared_ptr<arrow::Schema> schema;

        // Every field needs its name and data type.
        field_day = arrow::field("Day", arrow::int64());
        // The schema can be built from a vector of fields, and we do so here.
        schema = arrow::schema({field_day});

        std::shared_ptr<arrow::RecordBatch> rbatch;
        // The RecordBatch needs the schema, length for columns, which all must match,
        // and the actual data itself.
        rbatch = arrow::RecordBatch::Make(schema, days->length(), {days});
        return rbatch;
    }
    if (handle == "query3")
    {
        arrow::Int64Builder int64builder;
        std::array<int64_t, 100> days_raw{};
        createArray<100>(days_raw);
        // AppendValues, as called, puts 5 values from days_raw into our Builder object.
        ARROW_RETURN_NOT_OK(int64builder.AppendValues(days_raw.begin(), 100));
        std::shared_ptr<arrow::Array> days;
        ARROW_ASSIGN_OR_RAISE(days, int64builder.Finish());
        std::shared_ptr<arrow::Field> field_day;
        std::shared_ptr<arrow::Schema> schema;
        // Every field needs its name and data type.
        field_day = arrow::field("Day", arrow::int64());
        // The schema can be built from a vector of fields, and we do so here.
        schema = arrow::schema({field_day});
        std::shared_ptr<arrow::RecordBatch> rbatch;
        // The RecordBatch needs the schema, length for columns, which all must match,
        // and the actual data itself.
        rbatch = arrow::RecordBatch::Make(schema, days->length(), {days});
        return rbatch;
    }
    if (handle == "query4")
    {
        arrow::Int64Builder int64builder;
        std::array<int64_t, 1000> days_raw{};
        createArray<1000>(days_raw);
        // AppendValues, as called, puts 5 values from days_raw into our Builder object.
        ARROW_RETURN_NOT_OK(int64builder.AppendValues(days_raw.begin(), 1000));
        std::shared_ptr<arrow::Array> days;
        ARROW_ASSIGN_OR_RAISE(days, int64builder.Finish());
        std::shared_ptr<arrow::Field> field_day;
        std::shared_ptr<arrow::Schema> schema;
        // Every field needs its name and data type.
        field_day = arrow::field("Day", arrow::int64());

        // The schema can be built from a vector of fields, and we do so here.
        schema = arrow::schema({field_day});

        std::shared_ptr<arrow::RecordBatch> rbatch;
        // The RecordBatch needs the schema, length for columns, which all must match,
        // and the actual data itself.
        rbatch = arrow::RecordBatch::Make(schema, days->length(), {days});
        return rbatch;
    }
    if (handle == "query5")
    {
        arrow::Int64Builder int64builder;
        std::array<int64_t, 10000> days_raw{};
        createArray<10000>(days_raw);
        // AppendValues, as called, puts 5 values from days_raw into our Builder object.
        ARROW_RETURN_NOT_OK(int64builder.AppendValues(days_raw.begin(), 10000));
        std::shared_ptr<arrow::Array> days;
        ARROW_ASSIGN_OR_RAISE(days, int64builder.Finish());
        std::shared_ptr<arrow::Field> field_day;
        std::shared_ptr<arrow::Schema> schema;

        // Every field needs its name and data type.
        field_day = arrow::field("Day", arrow::int64());

        // The schema can be built from a vector of fields, and we do so here.
        schema = arrow::schema({field_day});

        std::shared_ptr<arrow::RecordBatch> rbatch;
        // The RecordBatch needs the schema, length for columns, which all must match,
        // and the actual data itself.
        rbatch = arrow::RecordBatch::Make(schema, days->length(), {days});
        return rbatch;
    }
    if (handle == "query6")
    {
        arrow::Int64Builder int64builder;
        std::array<int64_t, 100000> days_raw{};
        createArray<100000>(days_raw);
        // AppendValues, as called, puts 5 values from days_raw into our Builder object.
        ARROW_RETURN_NOT_OK(int64builder.AppendValues(days_raw.begin(), 100000));
        std::shared_ptr<arrow::Array> days;
        ARROW_ASSIGN_OR_RAISE(days, int64builder.Finish());
        std::shared_ptr<arrow::Field> field_day;
        std::shared_ptr<arrow::Schema> schema;

        // Every field needs its name and data type.
        field_day = arrow::field("Day", arrow::int64());

        // The schema can be built from a vector of fields, and we do so here.
        schema = arrow::schema({field_day});

        std::shared_ptr<arrow::RecordBatch> rbatch;
        // The RecordBatch needs the schema, length for columns, which all must match,
        // and the actual data itself.
        rbatch = arrow::RecordBatch::Make(schema, days->length(), {days});
        return rbatch;
    }
    if (handle == "query7")
    {
        arrow::Int64Builder int64builder;
        std::array<int64_t, 1000000> days_raw{};
        createArray<1000000>(days_raw);
        // AppendValues, as called, puts 5 values from days_raw into our Builder object.
        ARROW_RETURN_NOT_OK(int64builder.AppendValues(days_raw.begin(), 1000000));
        std::shared_ptr<arrow::Array> days;
        ARROW_ASSIGN_OR_RAISE(days, int64builder.Finish());
        std::shared_ptr<arrow::Field> field_day;
        std::shared_ptr<arrow::Schema> schema;
        // Every field needs its name and data type.
        field_day = arrow::field("Day", arrow::int64());
        // The schema can be built from a vector of fields, and we do so here.
        schema = arrow::schema({field_day});
        std::shared_ptr<arrow::RecordBatch> rbatch;
        // The RecordBatch needs the schema, length for columns, which all must match,
        // and the actual data itself.
        rbatch = arrow::RecordBatch::Make(schema, days->length(), {days});
        return rbatch;
    }
    }
    if (two_rows)
    {
        if (handle == "query1")
        {
            constexpr size_t size = 5;
            arrow::Int64Builder int64Builder;
            std::array<int64_t, size> values{};
            createArray<size>(values);
            // AppendValues, as called, puts 5 values from days_raw into our Builder object.
            ARROW_RETURN_NOT_OK(int64Builder.AppendValues(values.begin(), size));
            std::shared_ptr<arrow::Array> days;
            ARROW_ASSIGN_OR_RAISE(days, int64Builder.Finish());
            ARROW_RETURN_NOT_OK(int64Builder.AppendValues(values.begin(), size));
            std::shared_ptr<arrow::Array> months;
            ARROW_ASSIGN_OR_RAISE(months, int64Builder.Finish());
            std::shared_ptr<arrow::Field> field_day;
            std::shared_ptr<arrow::Field> field_month;
            std::shared_ptr<arrow::Schema> schema;

            // Every field needs its name and data type.
            field_day = arrow::field("Day", arrow::int64());
            field_month = arrow::field("Month", arrow::int64());

            // The schema can be built from a vector of fields, and we do so here.
            schema = arrow::schema({field_day, field_month});

            std::shared_ptr<arrow::RecordBatch> rbatch;
            // The RecordBatch needs the schema, length for columns, which all must match,
            // and the actual data itself.
            rbatch = arrow::RecordBatch::Make(schema, days->length(), {days, months});
            return rbatch;
        }
        if (handle == "query2")
        {
            constexpr size_t size = 5*10;
            arrow::Int64Builder int64Builder;
            std::array<int64_t, size> values{};
            createArray<size>(values);
            // AppendValues, as called, puts 5 values from days_raw into our Builder object.
            ARROW_RETURN_NOT_OK(int64Builder.AppendValues(values.begin(), size));
            std::shared_ptr<arrow::Array> days;
            ARROW_ASSIGN_OR_RAISE(days, int64Builder.Finish());
            ARROW_RETURN_NOT_OK(int64Builder.AppendValues(values.begin(), size));
            std::shared_ptr<arrow::Array> months;
            ARROW_ASSIGN_OR_RAISE(months, int64Builder.Finish());
            std::shared_ptr<arrow::Field> field_day;
            std::shared_ptr<arrow::Field> field_month;
            std::shared_ptr<arrow::Schema> schema;

            // Every field needs its name and data type.
            field_day = arrow::field("Day", arrow::int64());
            field_month = arrow::field("Month", arrow::int64());

            // The schema can be built from a vector of fields, and we do so here.
            schema = arrow::schema({field_day, field_month});

            std::shared_ptr<arrow::RecordBatch> rbatch;
            // The RecordBatch needs the schema, length for columns, which all must match,
            // and the actual data itself.
            rbatch = arrow::RecordBatch::Make(schema, days->length(), {days, months});
            return rbatch;
        }
        if (handle == "query3")
        {
            constexpr size_t size = 5*10*10;
            arrow::Int64Builder int64Builder;
            std::array<int64_t, size> values{};
            createArray<size>(values);
            // AppendValues, as called, puts 5 values from days_raw into our Builder object.
            ARROW_RETURN_NOT_OK(int64Builder.AppendValues(values.begin(), size));
            std::shared_ptr<arrow::Array> days;
            ARROW_ASSIGN_OR_RAISE(days, int64Builder.Finish());
            ARROW_RETURN_NOT_OK(int64Builder.AppendValues(values.begin(), size));
            std::shared_ptr<arrow::Array> months;
            ARROW_ASSIGN_OR_RAISE(months, int64Builder.Finish());
            std::shared_ptr<arrow::Field> field_day;
            std::shared_ptr<arrow::Field> field_month;
            std::shared_ptr<arrow::Schema> schema;

            // Every field needs its name and data type.
            field_day = arrow::field("Day", arrow::int64());
            field_month = arrow::field("Month", arrow::int64());

            // The schema can be built from a vector of fields, and we do so here.
            schema = arrow::schema({field_day, field_month});

            std::shared_ptr<arrow::RecordBatch> rbatch;
            // The RecordBatch needs the schema, length for columns, which all must match,
            // and the actual data itself.
            rbatch = arrow::RecordBatch::Make(schema, days->length(), {days, months});
            return rbatch;
        }
        if (handle == "query4")
        {
            constexpr size_t size = 5*10*10*10;
            arrow::Int64Builder int64Builder;
            std::array<int64_t, size> values{};
            createArray<size>(values);
            // AppendValues, as called, puts 5 values from days_raw into our Builder object.
            ARROW_RETURN_NOT_OK(int64Builder.AppendValues(values.begin(), size));
            std::shared_ptr<arrow::Array> days;
            ARROW_ASSIGN_OR_RAISE(days, int64Builder.Finish());
            ARROW_RETURN_NOT_OK(int64Builder.AppendValues(values.begin(), size));
            std::shared_ptr<arrow::Array> months;
            ARROW_ASSIGN_OR_RAISE(months, int64Builder.Finish());
            std::shared_ptr<arrow::Field> field_day;
            std::shared_ptr<arrow::Field> field_month;
            std::shared_ptr<arrow::Schema> schema;

            // Every field needs its name and data type.
            field_day = arrow::field("Day", arrow::int64());
            field_month = arrow::field("Month", arrow::int64());

            // The schema can be built from a vector of fields, and we do so here.
            schema = arrow::schema({field_day, field_month});

            std::shared_ptr<arrow::RecordBatch> rbatch;
            // The RecordBatch needs the schema, length for columns, which all must match,
            // and the actual data itself.
            rbatch = arrow::RecordBatch::Make(schema, days->length(), {days, months});
            return rbatch;
        }
        if (handle == "query5")
        {
            constexpr size_t size = 5*10*10*10*10;
            arrow::Int64Builder int64Builder;
            std::array<int64_t, size> values{};
            createArray<size>(values);
            // AppendValues, as called, puts 5 values from days_raw into our Builder object.
            ARROW_RETURN_NOT_OK(int64Builder.AppendValues(values.begin(), size));
            std::shared_ptr<arrow::Array> days;
            ARROW_ASSIGN_OR_RAISE(days, int64Builder.Finish());
            ARROW_RETURN_NOT_OK(int64Builder.AppendValues(values.begin(), size));
            std::shared_ptr<arrow::Array> months;
            ARROW_ASSIGN_OR_RAISE(months, int64Builder.Finish());
            std::shared_ptr<arrow::Field> field_day;
            std::shared_ptr<arrow::Field> field_month;
            std::shared_ptr<arrow::Schema> schema;

            // Every field needs its name and data type.
            field_day = arrow::field("Day", arrow::int64());
            field_month = arrow::field("Month", arrow::int64());

            // The schema can be built from a vector of fields, and we do so here.
            schema = arrow::schema({field_day, field_month});

            std::shared_ptr<arrow::RecordBatch> rbatch;
            // The RecordBatch needs the schema, length for columns, which all must match,
            // and the actual data itself.
            rbatch = arrow::RecordBatch::Make(schema, days->length(), {days, months});
            return rbatch;
        }
        if (handle == "query6")
        {
            constexpr size_t size =  5*10*10*10*10;
            arrow::Int64Builder int64Builder;
            std::array<int64_t, size> values{};
            createArray<size>(values);
            // AppendValues, as called, puts 5 values from days_raw into our Builder object.
            ARROW_RETURN_NOT_OK(int64Builder.AppendValues(values.begin(), size));
            std::shared_ptr<arrow::Array> array1;
            ARROW_ASSIGN_OR_RAISE(array1, int64Builder.Finish());

            ARROW_RETURN_NOT_OK(int64Builder.AppendValues(values.begin(), size));
            std::shared_ptr<arrow::Array> array2;
            ARROW_ASSIGN_OR_RAISE(array2, int64Builder.Finish());

            ARROW_RETURN_NOT_OK(int64Builder.AppendValues(values.begin(), size));
            std::shared_ptr<arrow::Array> array3;
            ARROW_ASSIGN_OR_RAISE(array3, int64Builder.Finish());

            ARROW_RETURN_NOT_OK(int64Builder.AppendValues(values.begin(), size));
            std::shared_ptr<arrow::Array> array4;
            ARROW_ASSIGN_OR_RAISE(array4, int64Builder.Finish());

            ARROW_RETURN_NOT_OK(int64Builder.AppendValues(values.begin(), size));
            std::shared_ptr<arrow::Array> array5;
            ARROW_ASSIGN_OR_RAISE(array5, int64Builder.Finish());

            ARROW_RETURN_NOT_OK(int64Builder.AppendValues(values.begin(), size));
            std::shared_ptr<arrow::Array> array6;
            ARROW_ASSIGN_OR_RAISE(array6, int64Builder.Finish());

            ARROW_RETURN_NOT_OK(int64Builder.AppendValues(values.begin(), size));
            std::shared_ptr<arrow::Array> array7;
            ARROW_ASSIGN_OR_RAISE(array7, int64Builder.Finish());

            ARROW_RETURN_NOT_OK(int64Builder.AppendValues(values.begin(), size));
            std::shared_ptr<arrow::Array> array8;
            ARROW_ASSIGN_OR_RAISE(array8, int64Builder.Finish());

            ARROW_RETURN_NOT_OK(int64Builder.AppendValues(values.begin(), size));
            std::shared_ptr<arrow::Array> array9;
            ARROW_ASSIGN_OR_RAISE(array9, int64Builder.Finish());

            ARROW_RETURN_NOT_OK(int64Builder.AppendValues(values.begin(), size));
            std::shared_ptr<arrow::Array> array10;
            ARROW_ASSIGN_OR_RAISE(array10, int64Builder.Finish());



            std::shared_ptr<arrow::Field> field;
            std::shared_ptr<arrow::Schema> schema;

            // Every field needs its name and data type.
            field = arrow::field("Day", arrow::int64());


            // The schema can be built from a vector of fields, and we do so here.
            schema = arrow::schema({field, field,field,field,field,field,field,field,field,field});

            std::shared_ptr<arrow::RecordBatch> rbatch;
            // The RecordBatch needs the schema, length for columns, which all must match,
            // and the actual data itself.
            rbatch = arrow::RecordBatch::Make(schema, array1->length(), {array1, array2, array3, array4, array5, array6, array7, array8, array9,array10});
            return rbatch;
        }


    }
    //String

    if (handle == "query1S")
    {
        arrow::StringBuilder stringBuilder;
        std::array<std::string, 1> values{};
        createArray<1>(values, 99);
        // AppendValues, as called, puts 5 values from days_raw into our Builder object.
        for (const auto& value : values)
        {
            ARROW_RETURN_NOT_OK(stringBuilder.Append(value));
        }
        std::shared_ptr<arrow::Array> days;
        ARROW_ASSIGN_OR_RAISE(days, stringBuilder.Finish());
        std::shared_ptr<arrow::Field> field_day;
        std::shared_ptr<arrow::Schema> schema;

        // Every field needs its name and data type.
        field_day = arrow::field("Day", arrow::utf8());

        // The schema can be built from a vector of fields, and we do so here.
        schema = arrow::schema({field_day});

        std::shared_ptr<arrow::RecordBatch> rbatch;
        // The RecordBatch needs the schema, length for columns, which all must match,
        // and the actual data itself.
        rbatch = arrow::RecordBatch::Make(schema, days->length(), {days});
        return rbatch;
    }
    if (handle == "query2S")
    {
        arrow::StringBuilder stringBuilder;
        std::array<std::string, 100> values{};
        createArray<100>(values, 99);
        // AppendValues, as called, puts 5 values from days_raw into our Builder object.
        for (const auto& value : values)
        {
            ARROW_RETURN_NOT_OK(stringBuilder.Append(value));
        }
        std::shared_ptr<arrow::Array> days;
        ARROW_ASSIGN_OR_RAISE(days, stringBuilder.Finish());
        std::shared_ptr<arrow::Field> field_day;
        std::shared_ptr<arrow::Schema> schema;

        // Every field needs its name and data type.
        field_day = arrow::field("Day",arrow::utf8());

        // The schema can be built from a vector of fields, and we do so here.
        schema = arrow::schema({field_day});

        std::shared_ptr<arrow::RecordBatch> rbatch;
        // The RecordBatch needs the schema, length for columns, which all must match,
        // and the actual data itself.
        rbatch = arrow::RecordBatch::Make(schema, days->length(), {days});
        return rbatch;
    }
    if (handle == "query3S")
    {
        arrow::StringBuilder stringBuilder;
        std::array<std::string, 1000> values{};
        createArray<1000>(values, 99);
        // AppendValues, as called, puts 5 values from days_raw into our Builder object.
        for (const auto& value : values)
        {
            ARROW_RETURN_NOT_OK(stringBuilder.Append(value));
        }
        std::shared_ptr<arrow::Array> days;
        ARROW_ASSIGN_OR_RAISE(days, stringBuilder.Finish());
        std::shared_ptr<arrow::Field> field_day;
        std::shared_ptr<arrow::Schema> schema;

        // Every field needs its name and data type.
        field_day = arrow::field("Day", arrow::utf8());

        // The schema can be built from a vector of fields, and we do so here.
        schema = arrow::schema({field_day});

        std::shared_ptr<arrow::RecordBatch> rbatch;
        // The RecordBatch needs the schema, length for columns, which all must match,
        // and the actual data itself.
        rbatch = arrow::RecordBatch::Make(schema, days->length(), {days});
        return rbatch;
    }
    if (handle == "query4S")
    {
        arrow::StringBuilder stringBuilder;
        std::array<std::string, 10000> values{};
        createArray<10000>(values, 99);
        // AppendValues, as called, puts 5 values from days_raw into our Builder object.
        for (const auto& value : values)
        {
            ARROW_RETURN_NOT_OK(stringBuilder.Append(value));
        }
        std::shared_ptr<arrow::Array> days;
        ARROW_ASSIGN_OR_RAISE(days, stringBuilder.Finish());
        std::shared_ptr<arrow::Field> field_day;
        std::shared_ptr<arrow::Schema> schema;

        // Every field needs its name and data type.
        field_day = arrow::field("Day", arrow::utf8());

        // The schema can be built from a vector of fields, and we do so here.
        schema = arrow::schema({field_day});

        std::shared_ptr<arrow::RecordBatch> rbatch;
        // The RecordBatch needs the schema, length for columns, which all must match,
        // and the actual data itself.
        rbatch = arrow::RecordBatch::Make(schema, days->length(), {days});
        return rbatch;
    }
    if (handle == "query5S")
    {
        arrow::StringBuilder stringBuilder;
        std::array<std::string, 100000> values{};
        createArray<100000>(values, 99);
        // AppendValues, as called, puts 5 values from days_raw into our Builder object.
        for (const auto& value : values)
        {
            ARROW_RETURN_NOT_OK(stringBuilder.Append(value));
        }
        std::shared_ptr<arrow::Array> days;
        ARROW_ASSIGN_OR_RAISE(days, stringBuilder.Finish());




        std::shared_ptr<arrow::Field> field_day;
        std::shared_ptr<arrow::Schema> schema;

        // Every field needs its name and data type.
        field_day = arrow::field("Day", arrow::utf8());

        // The schema can be built from a vector of fields, and we do so here.
        schema = arrow::schema({field_day});

        std::shared_ptr<arrow::RecordBatch> rbatch;
        // The RecordBatch needs the schema, length for columns, which all must match,
        // and the actual data itself.
        rbatch = arrow::RecordBatch::Make(schema, days->length(), {days});
        return rbatch;
    }

    return arrow::Status::Invalid("Not implemented");
}
