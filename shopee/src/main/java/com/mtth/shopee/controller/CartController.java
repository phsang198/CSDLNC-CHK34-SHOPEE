package com.mtth.shopee.controller;

import org.springframework.web.bind.annotation.*;
import redis.clients.jedis.Jedis;
import redis.clients.jedis.JedisPool;

import java.util.Map;

@RestController
@RequestMapping("/api/cart")
public class CartController {

    private final JedisPool jedisPool = new JedisPool("localhost", 6379);

    // retrive cart by userId
    @GetMapping("/{userId}")
    public Map<String, String> getCart(@PathVariable String userId) {
        try (Jedis jedis = jedisPool.getResource()) {
            String cartKey = "cart:" + userId;
            return jedis.hgetAll(cartKey);
        }
    }

    // add product to cart by userId
    @PostMapping("/{userId}/add")
    public String addToCart(@PathVariable String userId,
                            @RequestParam String productId,
                            @RequestParam int quantity) {
        try (Jedis jedis = jedisPool.getResource()) {
            String cartKey = "cart:" + userId;
            jedis.hset(cartKey, productId, String.valueOf(quantity));
            return "Product added/updated successfully!";
        }
    }

    // update product quantity in cart by userId
    @PostMapping("/{userId}/update")
    public String updateQuantity(@PathVariable String userId,
                                 @RequestParam String productId,
                                 @RequestParam int delta) {
        try (Jedis jedis = jedisPool.getResource()) {
            String cartKey = "cart:" + userId;
            jedis.hincrBy(cartKey, productId, delta);
            return "Product quantity updated successfully!";
        }
    }

    // delete product from cart by userId
    @DeleteMapping("/{userId}/remove")
    public String removeProduct(@PathVariable String userId,
                                @RequestParam String productId) {
        try (Jedis jedis = jedisPool.getResource()) {
            String cartKey = "cart:" + userId;
            jedis.hdel(cartKey, productId);
            return "Product removed successfully!";
        }
    }

    // delete all products from cart by userId
    @DeleteMapping("/{userId}")
    public String clearCart(@PathVariable String userId) {
        try (Jedis jedis = jedisPool.getResource()) {
            String cartKey = "cart:" + userId;
            jedis.del(cartKey);
            return "Cart cleared successfully!";
        }
    }
}
