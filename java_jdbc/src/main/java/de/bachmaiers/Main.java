package de.bachmaiers;

import java.io.IOException;
import java.sql.*;
import java.util.Properties;

enum QueryType {
    query1, query2, query3, query4, query5, query6, query7, query8,query9,query10;


    public String toQuery() {
        return switch (this) {
            case query1  -> "select * from lineitem LIMIT 1";
            case query2  -> "select * from lineitem LIMIT 100";
            case query3  -> "select * from lineitem LIMIT 1000";
            case query4  -> "select * from lineitem LIMIT 10000";
            case query5  -> "select * from lineitem LIMIT 100000";
            case query6  -> "select * from lineitem LIMIT 1000000";
            case query7  -> "select * from lineitem LIMIT 10000000";
            case query8  -> "select * from lineitem LIMIT 100000000";
            case query9  -> "select * from lineitem LIMIT 1000000000";
            case query10 -> "select * from lineitem LIMIT 10000000000";
            default -> throw new AssertionError();
        };
    }

    public int toIteration() {
        return switch (this) {
            case query1 -> 10_000 ;
            case query2 -> (100000 / 100) ;
            case query3 -> (100000 / 1000) ;
            case query4 -> (100000 / 10000) ;
            case query5 -> 10 ;
            case query6 -> 5 ;
            case query7 -> 2;
            case query8 -> 1;
            case query9 -> 1;
            case query10 -> 1;
            default -> throw new AssertionError();
        };
    }
}

public class Main {
    public static void main(String[] args) throws ClassNotFoundException, SQLException, InterruptedException {
        Class.forName("org.duckdb.DuckDBDriver");
        Class.forName("org.apache.arrow.driver.jdbc.ArrowFlightJdbcDriver");
        String url = System.getenv("JDBC_URL");

        System.out.println("Connecting to " + url);

        Connection conn = DriverManager.getConnection(url);
        for (int i = 1_000; i <= 1_000; i *= 10) {
            for (QueryType queryType : QueryType.values()) {
                Pair p = runTime(queryType.toQuery(), queryType.toIteration(), 0, 1_000_000_000, conn);
                System.out.println("Iteration " + queryType.toIteration() + " query: " + queryType.toString() + " : " + p.time + "  Size: "  + p.size);
            }

        }
        conn.close();


    }

    static  Pair runTime(String type, long iteration, long sleepBetween, double division, Connection conn) throws SQLException, InterruptedException {
        double elapsedSum = 0;
        long size = 0;
        for (int i = 0; i < iteration; i++) {
            size = 0;

            long start = System.nanoTime();
            PreparedStatement ps = conn.prepareStatement(type);
            ResultSet rs = ps.executeQuery();
            while (rs.next()) {
                size++;
            }
            rs.close();
            ps.close();
            long end = System.nanoTime();
            double elapsed = (end - start) / division;
            elapsedSum += elapsed;


        }
        return new Pair(elapsedSum, size);
    }

    static void resetCache() throws SQLException {
        try {
            // Run the sync command
            Runtime.getRuntime().exec("sudo sync").waitFor();
            // Run the command to clear caches
            Runtime.getRuntime().exec("free && sync && sudo sh -c 'echo 3 > /proc/sys/vm/drop_caches' && sync").waitFor();
            System.out.println("Cache cleared successfully.");
        } catch (IOException | InterruptedException e) {
            e.printStackTrace();
        }
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