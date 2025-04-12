// import {
//   IsString,
//   IsNumber,
//   IsEmail,
//   IsEnum,
//   IsOptional,
// } from 'class-validator';
// import { ComplaintType } from './create-complaint.dto';

// export class UpdateComplaintDto {
//   @IsString()
//   @IsOptional()
//   orderId?: string;

//   @IsString()
//   @IsOptional()
//   productId?: string;

//   @IsString()
//   @IsOptional()
//   buyerId?: string;

//   @IsString()
//   @IsOptional()
//   sellerId?: string;

//   @IsString()
//   @IsOptional()
//   reason?: string;

//   @IsString()
//   @IsOptional()
//   description?: string;

//   @IsString()
//   @IsOptional()
//   solution?: string;

//   @IsNumber()
//   @IsOptional()
//   refundAmount?: number;

//   @IsString()
//   @IsOptional()
//   refundDestination?: string;

//   @IsEmail()
//   @IsOptional()
//   email?: string;

//   @IsEnum(ComplaintType)
//   @IsOptional()
//   type?: ComplaintType;
// }
import {
  IsString,
  IsNumber,
  IsEmail,
  IsEnum,
  IsOptional,
} from 'class-validator';
import { ComplaintType } from './create-complaint.dto';

export class UpdateComplaintDto {
  @IsString()
  @IsOptional()
  orderId?: string;

  @IsString()
  @IsOptional()
  productId?: string;

  @IsString()
  @IsOptional()
  buyerId?: string;

  @IsString()
  @IsOptional()
  sellerId?: string;

  @IsString()
  @IsOptional()
  _from?: string;

  @IsString()
  @IsOptional()
  _to?: string;

  @IsString()
  @IsOptional()
  reason?: string;

  @IsString()
  @IsOptional()
  description?: string;

  @IsString()
  @IsOptional()
  solution?: string;

  @IsNumber()
  @IsOptional()
  refundAmount?: number;

  @IsString()
  @IsOptional()
  refundDestination?: string;

  @IsEmail()
  @IsOptional()
  email?: string;

  @IsEnum(ComplaintType)
  @IsOptional()
  type?: ComplaintType;
}
