// import { IsString, IsNumber, IsEmail, IsEnum } from 'class-validator';

// export enum ComplaintType {
//   DAMAGED = 'received_damaged',
//   NOT_RECEIVED = 'not_received',
//   MISSING_ITEM = 'missing_item',
// }

// export class CreateComplaintDto {
//   @IsString()
//   orderId: string;

//   @IsString()
//   productId: string;

//   @IsString()
//   buyerId: string;

//   @IsString()
//   sellerId: string;

//   @IsString()
//   reason: string;

//   @IsString()
//   description: string;

//   @IsString()
//   solution: string;

//   @IsNumber()
//   refundAmount: number;

//   @IsString()
//   refundDestination: string;

//   @IsEmail()
//   email: string;

//   @IsEnum(ComplaintType)
//   type: ComplaintType;
// }
import { IsString, IsNumber, IsEmail, IsEnum } from 'class-validator';

export enum ComplaintType {
  DAMAGED = 'received_damaged',
  NOT_RECEIVED = 'not_received',
  MISSING_ITEM = 'missing_item',
}

export class CreateComplaintDto {
  @IsString() orderId: string;

  @IsString() productId: string;

  @IsString() buyerId: string;

  @IsString() sellerId: string;

  @IsString() _from: string;

  @IsString() _to: string;

  @IsString() reason: string;

  @IsString() description: string;

  @IsString() solution: string;

  @IsNumber() refundAmount: number;

  @IsString() refundDestination: string;

  @IsEmail() email: string;

  @IsEnum(ComplaintType) type: ComplaintType;
}
