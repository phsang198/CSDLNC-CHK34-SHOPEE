package com.mtth.shopee.service;

import com.mtth.shopee.model.*;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;
import redis.clients.jedis.Jedis;
import redis.clients.jedis.JedisPool;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

@Service
@RequiredArgsConstructor
public class CartService {

    private final JedisPool jedisPool = new JedisPool("localhost", 6379);

    public Map<String, Integer> getCart(String userId) {
        try (Jedis jedis = jedisPool.getResource()) {
            Map<String, String> cart = jedis.hgetAll(buildCartKey(userId));
            Map<String, Integer> response = new HashMap<>();
            cart.forEach((k, v) -> response.put(k, Integer.parseInt(v)));
            return response;
        }
    }

    public void addToCart(String userId, String productId, int quantity) {
        try (Jedis jedis = jedisPool.getResource()) {
            jedis.hset(buildCartKey(userId), productId, String.valueOf(quantity));
        }
    }

    public void updateQuantity(String userId, String productId, int delta) {
        try (Jedis jedis = jedisPool.getResource()) {
            Long newQuantity = jedis.hincrBy(buildCartKey(userId), productId, delta);
            if (newQuantity <= 0) {
                jedis.hdel(buildCartKey(userId), productId);
            }
        }
    }

    public void removeProduct(String userId, String productId) {
        try (Jedis jedis = jedisPool.getResource()) {
            jedis.hdel(buildCartKey(userId), productId);
        }
    }

    public void clearCart(String userId) {
        try (Jedis jedis = jedisPool.getResource()) {
            jedis.del(buildCartKey(userId));
        }
    }

    public void addMultipleToCart(String userId, List<CartItem> items) {
        try (Jedis jedis = jedisPool.getResource()) {
            String cartKey = buildCartKey(userId);
            for (CartItem item : items) {
                jedis.hset(cartKey, item.getProductId(), String.valueOf(item.getQuantity()));
            }
        }
    }

    public CartSummary getCartSummary(String userId) {
        try (Jedis jedis = jedisPool.getResource()) {
            Map<String, String> cart = jedis.hgetAll(buildCartKey(userId));
            int totalQuantity = 0;
            double totalPrice = 0.0;

            for (Map.Entry<String, String> entry : cart.entrySet()) {
                int quantity = Integer.parseInt(entry.getValue());
                double pricePerProduct = 100.0; // TODO: Replace with product service
                totalQuantity += quantity;
                totalPrice += quantity * pricePerProduct;
            }

            return new CartSummary(totalQuantity, totalPrice);
        }
    }

    public void setCartExpiry(String userId) {
        try (Jedis jedis = jedisPool.getResource()) {
            jedis.expire(buildCartKey(userId), 86400); // 24 hours
        }
    }

    private String buildCartKey(String userId) {
        return "cart:" + userId;
    }
}
