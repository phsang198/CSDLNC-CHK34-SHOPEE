package com.mtth.shopee.controller;

import com.datastax.oss.driver.api.core.CqlSession;
import com.datastax.oss.driver.api.core.cql.*;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Component;

import java.net.InetSocketAddress;
import java.time.LocalDate;
import java.util.HashMap;
import java.util.Map;

@Component
public class CassandraAggregator {

    public CassandraAggregator() {
        System.out.println("✅ CassandraAggregator bean created!");
    }

    @Scheduled(fixedRate = 1800000) // 30 phút (30 phút = 30 * 60 * 1000 ms)
    public void aggregateData() {
        System.out.println("🔄 Starting Cassandra aggregation...");

        // 1. Kết nối tới Cassandra
        try (CqlSession session = CqlSession.builder()
                .addContactPoint(new InetSocketAddress("localhost", 9042))
                .withLocalDatacenter("datacenter1")
                .withKeyspace("shopee")
                .build()) {

            // Lấy ngày hiện tại
            LocalDate searchDate = LocalDate.now();
            // searchDate = LocalDate.parse("2025-04-05");

            // 2. Query data từ toàn bộ bảng search_logs_by_user_date
            String selectQuery = "SELECT search_query FROM search_logs_by_user_date WHERE search_date = ? ALLOW FILTERING";

            PreparedStatement preparedSelect = session.prepare(selectQuery);
            BoundStatement boundSelect = preparedSelect.bind(searchDate);

            ResultSet resultSet = session.execute(boundSelect);

            System.out.println("🔄 Aggregating data for date: " + resultSet);

            // 3. Đếm số lần xuất hiện của mỗi search_query
            Map<String, Integer> keywordCounts = new HashMap<>();
            for (Row row : resultSet) {
                String searchQuery = row.getString("search_query");
                keywordCounts.put(searchQuery, keywordCounts.getOrDefault(searchQuery, 0) + 1);
            }

            // 4. Insert kết quả vào popular_keywords_by_date
            String insertQuery = "INSERT INTO popular_keywords_by_date (search_date, search_query, total) VALUES (?, ?, ?)";
            PreparedStatement preparedInsert = session.prepare(insertQuery);

            for (Map.Entry<String, Integer> entry : keywordCounts.entrySet()) {
                BoundStatement boundInsert = preparedInsert.bind(
                        searchDate,
                        entry.getKey(),
                        entry.getValue()
                );
                session.execute(boundInsert);
                System.out.println("Inserted: " + entry.getKey() + " - " + entry.getValue());
            }

            System.out.println("✅ Aggregation completed successfully!");

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
