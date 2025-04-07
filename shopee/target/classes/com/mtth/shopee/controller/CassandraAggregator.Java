import com.datastax.oss.driver.api.core.CqlSession;
import com.datastax.oss.driver.api.core.cql.*;

import java.net.InetSocketAddress;
import java.time.LocalDate;
import java.util.HashMap;
import java.util.Map;

public class CassandraAggregator {
    public static void main(String[] args) {
        // 1. Kết nối tới Cassandra
        try (CqlSession session = CqlSession.builder()
                .addContactPoint(new InetSocketAddress("localhost", 9042))
                .withLocalDatacenter("datacenter1") // Thường là "datacenter1", kiểm tra bằng cqlsh: "describe keyspaces;"
                .withKeyspace("shopee")
                .build()) {

            String searchDate = "2025-04-05";
            String userId = "11111111-1111-1111-1111-111111111111";

            // 2. Query data từ search_logs_by_user_date
            String selectQuery = "SELECT search_query FROM search_logs_by_user_date WHERE user_id = ? AND search_date = ?";
            PreparedStatement preparedSelect = session.prepare(selectQuery);
            BoundStatement boundSelect = preparedSelect.bind(java.util.UUID.fromString(userId), LocalDate.parse(searchDate));

            ResultSet resultSet = session.execute(boundSelect);

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
                        LocalDate.parse(searchDate),
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
