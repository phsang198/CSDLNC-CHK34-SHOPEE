package com.mtth.shopee.controller;

import com.mtth.shopee.model.*;
import com.mtth.shopee.service.CartService;
import jakarta.validation.Valid;
import jakarta.validation.constraints.Min;
import jakarta.validation.constraints.NotBlank;
import lombok.RequiredArgsConstructor;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Map;
import java.util.UUID;

@RestController
@RequestMapping("/api/cart")
@RequiredArgsConstructor
@Validated
@CrossOrigin(origins = "http://localhost:3000")
public class CartController {

    private final CartService cartService;

    
    @GetMapping("/{userId}")
    public Map<String, Integer> getCart(@PathVariable @NotBlank UUID userId) {
        return cartService.getCart(userId);
    }

    
    @PostMapping("/{userId}/add")
    public String addToCart(@PathVariable @NotBlank UUID userId,
                            @RequestParam @NotBlank String productId,
                            @RequestParam @Min(1) int quantity) {
        cartService.addToCart(userId, productId, quantity);
        return "Product added/updated successfully!";
    }

    
    @PostMapping("/{userId}/update")
    public String updateQuantity(@PathVariable @NotBlank UUID userId,
                                 @RequestParam @NotBlank String productId,
                                 @RequestParam int delta) {
        cartService.updateQuantity(userId, productId, delta);
        return "Product quantity updated successfully!";
    }

    
    @DeleteMapping("/{userId}/remove")
    public String removeProduct(@PathVariable @NotBlank UUID userId,
                                @RequestParam @NotBlank String productId) {
        cartService.removeProduct(userId, productId);
        return "Product removed successfully!";
    }

    
    @DeleteMapping("/{userId}")
    public String clearCart(@PathVariable @NotBlank UUID userId) {
        cartService.clearCart(userId);
        return "Cart cleared successfully!";
    }

    
    @PostMapping("/{userId}/addBatch")
    public String addMultipleToCart(@PathVariable @NotBlank UUID userId,
                                    @RequestBody @Valid List<CartItem> items) {
        cartService.addMultipleToCart(userId, items);
        return "Batch products added/updated successfully!";
    }

    
    @GetMapping("/{userId}/summary")
    public CartSummary getCartSummary(@PathVariable @NotBlank UUID userId) {
        return cartService.getCartSummary(userId);
    }

   
    @PostMapping("/{userId}/expire")
    public String setCartExpiry(@PathVariable @NotBlank UUID userId) {
        cartService.setCartExpiry(userId);
        return "Cart expiry set to 24 hours!";
    }
}