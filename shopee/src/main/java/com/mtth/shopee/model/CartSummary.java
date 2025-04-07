package com.mtth.shopee.model;

import lombok.AllArgsConstructor;
import lombok.Data;

@Data
@AllArgsConstructor
public class CartSummary {
    private int totalQuantity;
    private double totalPrice;
}
