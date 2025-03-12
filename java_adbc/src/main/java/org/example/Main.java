package org.example;

import java.io.IOException;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;

import org.apache.arrow.adbc.core.AdbcConnection;
import org.apache.arrow.adbc.core.AdbcDatabase;
import org.apache.arrow.adbc.core.AdbcDriver;
import org.apache.arrow.adbc.core.AdbcException;
import org.apache.arrow.adbc.core.AdbcStatement;
import org.apache.arrow.adbc.driver.jdbc.JdbcDriver;
import org.apache.arrow.flatbuf.Decimal;
import org.apache.arrow.memory.BufferAllocator;
import org.apache.arrow.memory.RootAllocator;

enum QueryType {
    query1, query2, query3, query4, query5, query6, query7, query8;


    public String toQuery() {
        return switch (this) {
            case query1 -> "select * from lineitem LIMIT 1";
            case query2 -> "select * from lineitem LIMIT 100";
            case query3 -> "select * from lineitem LIMIT 1000";
            case query4 -> "select * from lineitem LIMIT 10000";
            case query5 -> "select * from lineitem LIMIT 100000";
            case query6 -> "select * from lineitem LIMIT 1000000";
            case query7 -> "select * from lineitem LIMIT 10000000";
            case query8 -> "select * from lineitem LIMIT 100000000";
            default -> throw new AssertionError();
        };
    }

    public int toIteration() {
        return switch (this) {
            case query1 -> 10_000;
            case query2 -> (100000 / 100);
            case query3 -> (100000 / 1000);
            case query4 -> (100000 / 10000);
            case query5 -> 10;
            case query6 -> 5;
            case query7 -> 2;
            case query8 -> 1;
            default -> throw new AssertionError();
        };
    }
}

public class Main {
    public static void main(String[] args) throws Exception {
        final Map<String, Object> parameters = new HashMap<>();
        String url = System.getenv("JDBC_URL");
        parameters.put(AdbcDriver.PARAM_URL, url);
        BufferAllocator allocator = new RootAllocator();
        AdbcDatabase db = new JdbcDriver(allocator).open(parameters);
        AdbcConnection adbcConnection = db.connect();




        for (int i = 1_000; i <= 1_000; i *= 10) {
            for (QueryType queryType : QueryType.values()) {
                Pair p = runTime(queryType.toString(), queryType.toIteration(), 0, 1_000_000_000, adbcConnection);
                System.out.println("Iteration " + queryType.toIteration() + " query: " + queryType.toString() + " : " + p.time + "  Size: " + p.size);
         
            }

        }

        adbcConnection.close();
        db.close();
        allocator.close();

    }

    static Pair runTime(String type, long iteration, long sleepBetween, double division, AdbcConnection adbcConnection) throws Exception {
        double elapsedSum = 0;
        long size = 0;
        for (int i = 0; i < iteration; i++) {
            size = 0;

            long start = System.nanoTime();
            AdbcStatement stmt = adbcConnection.createStatement();
            stmt.setSqlQuery(type);
            AdbcStatement.QueryResult queryResult = stmt.executeQuery();
            while (queryResult.getReader().loadNextBatch()) {
                size++;
            }
            for(Long t : queryResult.getReader().getDictionaryIds()) {
                queryResult.getReader().lookup(t).toString();
            }
            queryResult.close();
            stmt.close();


            long end = System.nanoTime();
            double elapsed = (end - start) / division;
            elapsedSum += elapsed;


        }
        return new Pair(elapsedSum, size);
    }
}

class Pair {
    double time;
    long size;

    Pair(double time, long size) {
        this.time = time;
        this.size = size;
    }
}