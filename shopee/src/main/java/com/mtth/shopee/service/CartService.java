package com.mtth.shopee.service;

import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.mtth.shopee.model.*;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;
import redis.clients.jedis.Jedis;
import redis.clients.jedis.JedisPool;

import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.UUID;

@Service
@RequiredArgsConstructor
public class CartService {

    private final JedisPool jedisPool = new JedisPool("localhost", 6379);
    private final ObjectMapper objectMapper = new ObjectMapper();

    public Map<String, Integer> getCart(UUID userId) {
        try (Jedis jedis = jedisPool.getResource()) {
            String json = jedis.get(buildCartKey(userId));
            if (json == null || json.isEmpty()) {
                return new HashMap<>();
            }
            return objectMapper.readValue(json, new TypeReference<Map<String, Integer>>() {});
        } catch (IOException e) {
            throw new RuntimeException("Failed to parse cart JSON", e);
        }
    }

    public void addToCart(UUID userId, String productId, int quantity) {
        try (Jedis jedis = jedisPool.getResource()) {
            Map<String, Integer> cart = getCart(userId);
            cart.put(productId, quantity);
            jedis.set(buildCartKey(userId), objectMapper.writeValueAsString(cart));
        } catch (IOException e) {
            throw new RuntimeException("Failed to write cart JSON", e);
        }
    }

    public void updateQuantity(UUID userId, String productId, int delta) {
        try (Jedis jedis = jedisPool.getResource()) {
            Map<String, Integer> cart = getCart(userId);
            int newQuantity = cart.getOrDefault(productId, 0) + delta;
            if (newQuantity <= 0) {
                cart.remove(productId);
            } else {
                cart.put(productId, newQuantity);
            }
            jedis.set(buildCartKey(userId), objectMapper.writeValueAsString(cart));
        } catch (IOException e) {
            throw new RuntimeException("Failed to write cart JSON", e);
        }
    }

    public void removeProduct(UUID userId, String productId) {
        try (Jedis jedis = jedisPool.getResource()) {
            Map<String, Integer> cart = getCart(userId);
            cart.remove(productId);
            jedis.set(buildCartKey(userId), objectMapper.writeValueAsString(cart));
        } catch (IOException e) {
            throw new RuntimeException("Failed to write cart JSON", e);
        }
    }

    public void clearCart(UUID userId) {
        try (Jedis jedis = jedisPool.getResource()) {
            jedis.del(buildCartKey(userId));
        }
    }

    public void addMultipleToCart(UUID userId, List<CartItem> items) {
        try (Jedis jedis = jedisPool.getResource()) {
            Map<String, Integer> cart = getCart(userId);
            for (CartItem item : items) {
                cart.put(item.getProductId(), item.getQuantity());
            }
            jedis.set(buildCartKey(userId), objectMapper.writeValueAsString(cart));
        } catch (IOException e) {
            throw new RuntimeException("Failed to write cart JSON", e);
        }
    }

    public CartSummary getCartSummary(UUID userId) {
        Map<String, Integer> cart = getCart(userId);
        int totalQuantity = 0;
        double totalPrice = 0.0;

        for (Map.Entry<String, Integer> entry : cart.entrySet()) {
            int quantity = entry.getValue();
            double pricePerProduct = 100.0; // TODO: Replace with real product service
            totalQuantity += quantity;
            totalPrice += quantity * pricePerProduct;
        }

        return new CartSummary(totalQuantity, totalPrice);
    }

    public void setCartExpiry(UUID userId) {
        try (Jedis jedis = jedisPool.getResource()) {
            jedis.expire(buildCartKey(userId), 86400); // 24 hours
        }
    }

    private String buildCartKey(UUID userId) {
        return "cart:" + userId;
    }
}
