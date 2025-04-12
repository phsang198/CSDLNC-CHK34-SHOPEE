import { IsString, IsNumber, IsEmail, IsEnum } from 'class-validator';

export class CreateResponseDto {
  @IsString() conplaintId: string;

  @IsString() sellerId: string;

  @IsString() _from: string;

  @IsString() _to: string;

  @IsString() reason: string;

  @IsString() description: string;

  @IsString() solution: string;
}
